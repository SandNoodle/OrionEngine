#if defined(ORION_PLATFORM_LINUX)
#include "Platform/Platform.h"

#include "Core/Assert.h"
#include "Core/Standard/Memory/Allocators/ArenaAllocator.h"

#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <unistd.h>

namespace Orion::Engine::Platform
{
	static Memory::ArenaAllocator k_platform_thread_allocator;

	struct PlatformThread
	{
		PlatformThreadFn function;
		void* function_user_data;
		pthread_t handle;
	};

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
		return (PlatformInfo){
			.system_name              = StringView("Linux"),
			.page_size_in_bytes       = static_cast<UInt64>(getpagesize()),
			.large_page_size_in_bytes = ORION_MEGABYTES(2),
			.logical_processor_count  = static_cast<UInt32>(get_nprocs()),
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
			.file_name               = String(native_path),
			.size_in_bytes           = static_cast<UInt64>(file_stat.st_size),
			.unix_time_created       = static_cast<UInt64>(file_stat.st_ctim.tv_sec),
			.unix_time_last_modified = static_cast<UInt64>(file_stat.st_mtim.tv_sec),
			.unix_time_last_accessed = static_cast<UInt64>(file_stat.st_atim.tv_sec),
			.access_flags            = access_flags,
		};
	}

	static constexpr void DoListFiles(Vector<PlatformFileStat>& file_stats,
	                                  StringView path,
	                                  PlatformListOption list_option) noexcept
	{
		ORION_ASSERT_DEBUG(path.Size() > 0);

		CString native_path = SanitizePath(path);
		DIR* base_directory = opendir(native_path);
		if (!base_directory) [[unlikely]] {
			return;
		}

		dirent* entry = nullptr;
		while ((entry = readdir(base_directory)) != nullptr) {
			StringView entry_name = StringView(entry->d_name);
			if (entry->d_type == DT_DIR) {
				if (entry_name == StringView("..") || entry_name == StringView(".")) {
					continue;
				}

				if (list_option == PlatformListOption::Recursive) {
					String path_buffer{};
					StringView sub_path = CombinePath(path_buffer, path, entry_name);
					DoListFiles(file_stats, sub_path, list_option);
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

	Vector<PlatformFileStat> ListFiles(StringView path, PlatformListOption list_option) noexcept
	{
		Vector<PlatformFileStat> result{};
		DoListFiles(result, path, list_option);
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

	static void* ThreadFunctionWrapper(void* user_data) noexcept
	{
		PlatformThreadHandle thread = static_cast<PlatformThreadHandle>(user_data);
		thread->function(user_data);
		return nullptr;
	}

	PlatformThreadHandle ThreadCreate(PlatformThreadFn thread_function, void* user_data) noexcept
	{
		ORION_ASSERT_DEBUG(thread_function);

		PlatformThreadHandle thread = Memory::AllocateConstruct<PlatformThread>(k_platform_thread_allocator);
		thread->function            = thread_function;
		thread->function_user_data  = user_data;

		if (pthread_create(&thread->handle, nullptr, ThreadFunctionWrapper, thread) != 0) {
			Memory::FreeDestruct<PlatformThread>(k_platform_thread_allocator, thread);
			return nullptr;
		}
		return thread;
	}

	void ThreadDestroy(PlatformThreadHandle thread) noexcept
	{
		if (thread && thread->handle) {
			// NOTE: pthread_cancel returns an error code, which we can't handle anyway - perform the cleanup as normal.
			pthread_cancel(thread->handle);
			thread->function           = nullptr;
			thread->function_user_data = nullptr;
			thread->handle             = 0UL;
			Memory::FreeDestruct<PlatformThread>(k_platform_thread_allocator, thread);
		}
	}

	void ThreadJoin(PlatformThreadHandle thread) noexcept
	{
		if (thread && thread->handle) {
			ORION_NOT_IMPLEMENTED();
		}
	}

	void ThreadDetach(PlatformThreadHandle thread) noexcept
	{
		if (thread && thread->handle) {
			ORION_NOT_IMPLEMENTED();
		}
	}
}  // namespace Orion::Engine::Platform

#endif  // ORION_PLATFORM_LINUX
