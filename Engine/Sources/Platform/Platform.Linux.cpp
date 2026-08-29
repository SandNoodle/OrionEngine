#if defined(ORION_PLATFORM_LINUX)
#include "Platform/Platform.h"

#include "Core/Assert.h"

#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>

namespace Orion::Engine::Platform
{
	[[nodiscard]] static constexpr CString SanitizePath(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		ORION_ASSERT_DEBUG(path.Size() <= PATH_MAX);

		static String path_buffer{};
		path_buffer.Clear();
		path_buffer.Reserve(path.Size() + 1);
		path_buffer.AppendRange(path.begin(), path.end());
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
		// TODO(SandNoodle): We need a reliable way to get the system name (right now its HARDCODED).
		return (PlatformInfo){
			.system_name             = ORION_STRINGVIEW("Linux"),
			.page_size               = static_cast<UInt64>(getpagesize()),
			.large_page_size         = ORION_MEGABYTES(2),
			.logical_processor_count = static_cast<UInt32>(get_nprocs()),
		};
	}

	[[nodiscard]] static constexpr Int32 ToNativeFlags(PlatformFileAccessFlags flags) noexcept
	{
		if (EnumHasAllFlags(flags, PlatformFileAccessFlags::All)) {
			return O_RDWR;
		}

		if (EnumHasAnyFlags(flags, PlatformFileAccessFlags::Read)) {
			return O_RDONLY;
		}

		if (EnumHasAnyFlags(flags, PlatformFileAccessFlags::Write)) {
			return O_WRONLY;
		}

		ORION_NOT_IMPLEMENTED("unhandled PlatformFileAccessFlags case");
		return 0;
	}

	Bool8 FileCreate(StringView path, PlatformFileAccessFlags flags) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		// NOTE: We need a small workaround here, as Linux opens a file immediately after its creation - we have to
		//       close it manually afterwards. Also, we should fail if the file already exists (to preserve consistent
		//       behavior across all platforms).
		CString native_path      = SanitizePath(path);
		const Int32 native_flags = ToNativeFlags(flags) | O_CREAT | O_EXCL;
		if (Int32 file_descriptor = open(native_path, native_flags); file_descriptor >= 0) {
			close(file_descriptor);
			return true;
		}

		return false;
	}

	Bool8 FileRemove(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		return unlink(native_path) == 0;
	}

	Bool8 FileExists(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		struct stat file_stat{};
		return stat(native_path, &file_stat) == 0 && (S_ISREG(file_stat.st_mode) || S_ISDIR(file_stat.st_mode));
	}

	PlatformFileStat StatFile(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		CString native_path = SanitizePath(path);
		struct stat file_stat{};
		Int32 stat_result = stat(native_path, &file_stat);
		ORION_ASSERT(stat_result == 0, "[Platform] Cannot stat the file ('{}'), because it does not exist.", path);

		PlatformFileAccessFlags access_flags = PlatformFileAccessFlags::None;
		access_flags |= file_stat.st_mode & S_IREAD ? PlatformFileAccessFlags::Read : PlatformFileAccessFlags::None;
		access_flags |= file_stat.st_mode & S_IWUSR ? PlatformFileAccessFlags::Write : PlatformFileAccessFlags::None;

		return (PlatformFileStat){
			.file_name          = ORION_STRING(native_path),
			.size_in_bytes      = static_cast<UInt64>(file_stat.st_size),
			.time_created       = static_cast<UInt64>(file_stat.st_ctim.tv_sec),
			.time_last_modified = static_cast<UInt64>(file_stat.st_mtim.tv_sec),
			.time_last_accessed = static_cast<UInt64>(file_stat.st_atim.tv_sec),
			.access_flags       = access_flags,
		};
	}

	static constexpr void DoListFiles(Vector<PlatformFileStat>& file_stats, StringView path, Bool8 recursive) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		CString native_path = SanitizePath(path);
		DIR* base_directory = opendir(native_path);
		if (!base_directory) [[unlikely]] {
			return;
		}

		dirent* entry = nullptr;
		while ((entry = readdir(base_directory)) != nullptr) {
			StringView entry_name = ORION_STRINGVIEW(entry->d_name);
			if (entry->d_type == DT_DIR) {
				if (entry_name == ORION_STRINGVIEW("..") || entry_name == ORION_STRINGVIEW(".")) {
					continue;
				}

				if (recursive) {
					String path_buffer{};
					StringView sub_path = CombinePath(path_buffer, path, entry_name);
					DoListFiles(file_stats, sub_path, recursive);
				}
			}

			if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
				String path_buffer{};
				StringView file_path = CombinePath(path_buffer, path, entry_name);
				file_stats.AddConstruct(StatFile(file_path));
			}
		}

		closedir(base_directory);
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
		return mkdir(native_path, 0777) == 0;
	}

	Bool8 DirectoryRemove(StringView path) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);
		CString native_path = SanitizePath(path);
		return rmdir(native_path) == 0;
	}

	Bool8 DirectoryExists(StringView path) noexcept
	{
		CString native_path = SanitizePath(path);
		ORION_ASSERT_DEBUG(path.Size() > 0);
		struct stat file_stat{};
		return stat(native_path, &file_stat) == 0 && (file_stat.st_mode & S_IFMT) == S_IFDIR;
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_LINUX
