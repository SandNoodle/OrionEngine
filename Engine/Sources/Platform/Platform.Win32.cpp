#if defined(ORION_PLATFORM_WINDOWS)
#include "Platform/Platform.h"

#include "Core/Standard/Containers/Array.h"

#include <fileapi.h>
#include <handleapi.h>
#include <memoryapi.h>
#include <sysinfoapi.h>

namespace Orion::Engine::Platform
{
	[[nodiscard]] static constexpr CString SanitizePath(StringView path, StringView suffix = StringView()) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		ORION_ASSERT_DEBUG(path.Size() <= MAX_PATH);

		static String path_buffer{};
		path_buffer.Clear();
		path_buffer.Reserve(path.Size() + 1);
		path_buffer.AppendRange(path.begin(), path.end());
		if (!suffix.IsEmpty()) {
			path_buffer.AppendRange(suffix.begin(), suffix.end());
		}
		path_buffer.Append(static_cast<String::WideCharType>('\0'));  // Null terminator.
		return reinterpret_cast<CString>(path_buffer.Data());
	}

	[[nodiscard]] static constexpr StringView CombinePath(String& path_buffer,
	                                                      StringView base_path,
	                                                      StringView sub_path) noexcept
	{
		path_buffer.AppendRange(base_path.begin(), base_path.end());
		if (path_buffer.Back() != '/') {
			path_buffer.Append('/');
		}
		path_buffer.AppendRange(sub_path.begin(), sub_path.end());
		return StringView(path_buffer.begin(), path_buffer.end());
	}

	PlatformInfo GetPlatformInfo() noexcept
	{
		// System capabilities.
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);

		// TODO(SandNoodle): We need a reliable way to get the system name (right now its HARDCODED).
		return (PlatformInfo){
			.system_name             = ORION_STRINGVIEW("Windows"),
			.page_size               = static_cast<UInt64>(system_info.dwPageSize),
			.large_page_size         = static_cast<UInt64>(GetLargePageMinimum()),
			.logical_processor_count = static_cast<UInt32>(system_info.dwNumberOfProcessors),
		};
	}
	[[nodiscard]] static constexpr DWORD ToNativeFlags(PlatformFileAccessFlags flags) noexcept
	{
		if (EnumHasAllFlags(flags, PlatformFileAccessFlags::All)) {
			return GENERIC_READ | GENERIC_WRITE;
		}

		if (EnumHasAnyFlags(flags, PlatformFileAccessFlags::Read)) {
			return GENERIC_READ;
		}

		if (EnumHasAnyFlags(flags, PlatformFileAccessFlags::Write)) {
			return GENERIC_WRITE;
		}

		ORION_NOT_IMPLEMENTED("unhandled PlatformFileAccessFlags case");
		return 0;
	}
	[[nodiscard]] static constexpr DWORD ToNativeAttributes(PlatformFileAccessFlags flags) noexcept
	{
		DWORD attributes     = FILE_ATTRIBUTE_NORMAL;
		Bool8 has_read_flag  = EnumHasAllFlags(flags, PlatformFileAccessFlags::Read);
		Bool8 has_write_flag = EnumHasAllFlags(flags, PlatformFileAccessFlags::Write);
		if (has_read_flag && !has_write_flag) {
			attributes |= FILE_ATTRIBUTE_READONLY;
		}
		return attributes;
	}

	Bool8 FileCreate(StringView path, PlatformFileAccessFlags flags) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		// NOTE: We need a small workaround here, as Windows opens a file immediately after its creation - we have to
		//       close it manually afterwards. Also, we should fail if the file already exists (to preserve consistent
		//       behavior across all platforms).
		CString native_path                       = SanitizePath(path);
		DWORD native_flags                        = ToNativeFlags(flags);
		DWORD native_attributes                   = ToNativeAttributes(flags);
		DWORD share_mode                          = 0UL;
		LPSECURITY_ATTRIBUTES security_attributes = nullptr;
		DWORD creation_disposition                = CREATE_NEW;
		HANDLE template_file                      = nullptr;
		if (HANDLE file_handle = CreateFile(native_path,
		                                    native_flags,
		                                    share_mode,
		                                    security_attributes,
		                                    creation_disposition,
		                                    native_attributes,
		                                    template_file);
		    file_handle != INVALID_HANDLE_VALUE) {
			CloseHandle(file_handle);
			return true;
		}

		return false;
	}

	Bool8 FileRemove(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		return DeleteFile(native_path) != 0;
	}

	Bool8 FileExists(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		DWORD attributes    = GetFileAttributes(native_path);
		return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	[[nodiscard]] static constexpr UInt64 ToUnixTime(DWORD low, DWORD high) noexcept
	{
		ULARGE_INTEGER foo{
			.LowPart  = low,
			.HighPart = high,
		};
		return (foo.QuadPart - 116444736000000000) / 10000000;
	}

	PlatformFileStat StatFile(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		WIN32_FIND_DATAA file_data;
		CString native_path = SanitizePath(path);
		HANDLE file_handle  = FindFirstFile(native_path, &file_data);
		ORION_ASSERT(file_handle != INVALID_HANDLE_VALUE,
		             "[Platform] Cannot stat the file ('{}'), because it does not exist.",
		             path);

		// TODO(SandNoodle): What about files we cannot read nor write?
		PlatformFileAccessFlags access_flags = !(file_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
		                                         ? PlatformFileAccessFlags::All
		                                         : PlatformFileAccessFlags::Read;

		ULARGE_INTEGER size_in_bytes{
			.LowPart  = file_data.nFileSizeLow,
			.HighPart = file_data.nFileSizeHigh,
		};

		FindClose(file_handle);

		return (PlatformFileStat){
			.file_name          = ORION_STRING(native_path),
			.size_in_bytes      = size_in_bytes.QuadPart,
			.time_created       = ToUnixTime(file_data.ftCreationTime.dwLowDateTime,  //
                                       file_data.ftCreationTime.dwHighDateTime),
			.time_last_modified = ToUnixTime(file_data.ftLastWriteTime.dwLowDateTime,  //
			                                 file_data.ftLastWriteTime.dwHighDateTime),
			.time_last_accessed = ToUnixTime(file_data.ftLastAccessTime.dwLowDateTime,  //
			                                 file_data.ftLastAccessTime.dwHighDateTime),
			.access_flags       = access_flags,
		};
	}

	static constexpr void DoListFiles(Vector<PlatformFileStat>& file_stats, StringView path, Bool8 recursive) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		WIN32_FIND_DATA fd{};
		CString native_path = SanitizePath(path, ORION_STRINGVIEW("/*"));
		HANDLE file_handle  = FindFirstFile(native_path, &fd);
		if (file_handle == INVALID_HANDLE_VALUE) {
			return;
		}

		do {
			if (fd.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
				continue;
			}

			StringView entry_name = ORION_STRINGVIEW(fd.cFileName);
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (entry_name == ORION_STRINGVIEW(".") || entry_name == ORION_STRINGVIEW("..")) {
					continue;
				}

				if (recursive) {
					String path_buffer{};
					StringView sub_path = CombinePath(path_buffer, path, entry_name);
					DoListFiles(file_stats, sub_path, recursive);
				}
			}

			static constexpr DWORD k_possible_file_flags
				= FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_REPARSE_POINT;
			if (BitFlagHasAnyFlags(fd.dwFileAttributes, k_possible_file_flags)) {
				// TODO(SandNoodle): What about files we cannot read nor write?
				PlatformFileAccessFlags access_flags = !(fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				                                         ? PlatformFileAccessFlags::All
				                                         : PlatformFileAccessFlags::Read;

				ULARGE_INTEGER size_in_bytes{
					.LowPart  = fd.nFileSizeLow,
					.HighPart = fd.nFileSizeHigh,
				};

				String path_buffer{};
				StringView file_path = CombinePath(path_buffer, path, entry_name);
				file_stats.AddConstruct((PlatformFileStat){
					.file_name          = String(file_path.begin(), file_path.Size()),
					.size_in_bytes      = size_in_bytes.QuadPart,
					.time_created       = ToUnixTime(fd.ftCreationTime.dwLowDateTime,  //
                                               fd.ftCreationTime.dwHighDateTime),
					.time_last_modified = ToUnixTime(fd.ftLastWriteTime.dwLowDateTime,  //
				                                     fd.ftLastWriteTime.dwHighDateTime),
					.time_last_accessed = ToUnixTime(fd.ftLastAccessTime.dwLowDateTime,  //
				                                     fd.ftLastAccessTime.dwHighDateTime),
					.access_flags       = access_flags,
				});
			}
		} while (FindNextFile(file_handle, &fd) != 0);

		FindClose(file_handle);
	}

	Vector<PlatformFileStat> ListFiles(StringView path, Bool8 recursive) noexcept
	{
		Vector<PlatformFileStat> result{};
		DoListFiles(result, path, recursive);
		return result;
	}

	Bool8 DirectoryCreate(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		return CreateDirectory(native_path, nullptr) != 0;
	}

	Bool8 DirectoryRemove(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		return RemoveDirectory(native_path) != 0;
	}

	Bool8 DirectoryExists(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		CString native_path = SanitizePath(path);
		DWORD attributes    = GetFileAttributes(native_path);
		return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_WINDOWS
