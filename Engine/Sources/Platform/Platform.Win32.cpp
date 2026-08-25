#if defined(ORION_PLATFORM_WINDOWS)
#include "Platform.h"
#include "Platform/Platform.h"

#include "Core/Standard/Containers/Array.h"

#include <fileapi.h>
#include <handleapi.h>
#include <memoryapi.h>
#include <sysinfoapi.h>

namespace Orion::Engine::Platform
{
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

	Bool8 FileExists(CString path) noexcept
	{
		ORION_ASSERT_DEBUG(path);
		DWORD attributes = GetFileAttributes(path);
		return attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	Bool8 DirectoryExists(CString path) noexcept
	{
		ORION_ASSERT_DEBUG(path);
		DWORD attributes = GetFileAttributes(path);
		return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	[[nodiscard]] static constexpr UInt64 ToUnixTime(DWORD low, DWORD high) noexcept
	{
		ULARGE_INTEGER foo{
			.LowPart  = low,
			.HighPart = high,
		};
		return (foo.QuadPart - 116444736000000000) / 10000000;
	}

	PlatformFileStat StatFile(CString path) noexcept
	{
		ORION_ASSERT_DEBUG(path);

		WIN32_FIND_DATAA file_data;
		HANDLE file_handle = FindFirstFile(path, &file_data);
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

		return (PlatformFileStat){
			.file_name          = ORION_STRING(path),
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
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_WINDOWS
