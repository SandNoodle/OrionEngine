#if defined(ORION_PLATFORM_LINUX)
#include "Platform/Platform.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>

namespace Orion::Engine::Platform
{
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

	Bool8 FileCreate(CString path, PlatformFileAccessFlags flags) noexcept
	{
		ORION_ASSERT_DEBUG(path);

		// NOTE: We need a small workaround here, as Linux opens a file immediately after its creation - we have to
		//       close it manually afterwards. Also, we should fail if the file already exists (to preserve consistent
		//       behavior across all platforms).
		const Int32 native_flags = ToNativeFlags(flags) | O_CREAT | O_EXCL;
		if (Int32 file_descriptor = open(path, native_flags); file_descriptor != 0) {
			close(file_descriptor);
			return true;
		}

		return false;
	}

	Bool8 FileRemove(CString path) noexcept
	{
		ORION_ASSERT_DEBUG(path);
		return unlink(path) == 0;
	}

	Bool8 FileExists(CString path) noexcept
	{
		struct stat file_stat{};
		return stat(path, &file_stat) == 0 && (S_ISREG(file_stat.st_mode) || S_ISDIR(file_stat.st_mode));
	}

	PlatformFileStat StatFile(CString path) noexcept
	{
		ORION_ASSERT_DEBUG(path);

		struct stat file_stat{};
		Int32 stat_result = stat(path, &file_stat);
		ORION_ASSERT(stat_result == 0, "[Platform] Cannot stat the file ('{}'), because it does not exist.", path);

		PlatformFileAccessFlags access_flags = PlatformFileAccessFlags::None;
		access_flags |= file_stat.st_mode & S_IREAD ? PlatformFileAccessFlags::Read : PlatformFileAccessFlags::None;
		access_flags |= file_stat.st_mode & S_IWUSR ? PlatformFileAccessFlags::Write : PlatformFileAccessFlags::None;

		return (PlatformFileStat){
			.file_name          = ORION_STRING(path),
			.size_in_bytes      = static_cast<UInt64>(file_stat.st_size),
			.time_created       = static_cast<UInt64>(file_stat.st_ctim.tv_sec),
			.time_last_modified = static_cast<UInt64>(file_stat.st_mtim.tv_sec),
			.time_last_accessed = static_cast<UInt64>(file_stat.st_atim.tv_sec),
			.access_flags       = access_flags,
		};
	}

	Vector<PlatformFileStat> ListFiles(CString path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryCreate(CString path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryRemove(CString path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Bool8 DirectoryExists(CString path) noexcept
	{
		struct stat file_stat{};
		return stat(path, &file_stat) == 0 && (file_stat.st_mode & S_IFMT) == S_IFDIR;
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_LINUX
