#include "Platform/FileSystem/Storage/LocalStorage.h"

#include "Core/Standard/Utility/MoveAndForward.h"
#include "Platform/Platform.h"

namespace Orion::Engine::Platform::FileSystem
{
	LocalStorageFileWriter::LocalStorageFileWriter(StringView path)
	{
		ORION_IGNORE_PARAM(path);
	}

	StringView LocalStorageProvider::Protocol() noexcept
	{
		return StringView("local://");
	}

	Optional<IOError> LocalStorageProvider::Create(StringView path) noexcept
	{
		if (Optional<IOError> ensure_directory_structure_result = EnsureDirectoryStructure(path);
		    ensure_directory_structure_result.IsValue()) {
			return ensure_directory_structure_result;
		}

		if (!FileCreate(path, PlatformFileAccessFlags::All)) {
			return IOError::FileCreationFailed;
		}
		return k_null_option;
	}

	Optional<IOError> LocalStorageProvider::Remove(StringView path) noexcept
	{
		if (!FileRemove(path)) {
			return IOError::FileDeletionFailed;
		}
		return k_null_option;
	}

	IOResult<IStorageFileWriter*> LocalStorageProvider::Write(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
		return nullptr;
	}

	IOResult<IStorageFileReader*> LocalStorageProvider::Read(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	IOResult<StorageStatInfo> LocalStorageProvider::Stat(StringView path) noexcept
	{
		if (!FileExists(path)) {
			return IOError::FileDoesNotExist;
		}

		PlatformFileStat platform_file_stat = StatFile(path);
		return (StorageStatInfo){
			.file_name               = Move(platform_file_stat.file_name),
			.size_in_bytes           = platform_file_stat.size_in_bytes,
			.unix_time_created       = platform_file_stat.unix_time_created,
			.unix_time_last_accessed = platform_file_stat.unix_time_last_accessed,
			.unix_time_last_modified = platform_file_stat.unix_time_last_modified,
		};
	}

	Vector<StorageStatInfo> LocalStorageProvider::List(StringView path, StorageListOption list_option) noexcept
	{
		PlatformListOption platform_list_option = [list_option]() -> PlatformListOption {
			switch (list_option) {
				case StorageListOption::NonRecursive:
					return PlatformListOption::NonRecursive;
				case StorageListOption::Recursive:
					return PlatformListOption::Recursive;
				default:
					ORION_NOT_IMPLEMENTED("unhandled StorageListOption case.");
					return PlatformListOption::NonRecursive;
			}
		}();

		Vector<PlatformFileStat> platform_files = ListFiles(path, platform_list_option);
		Vector<StorageStatInfo> result{};
		result.Reserve(platform_files.Size());
		for (USize index = 0; index < platform_files.Size(); ++index) {
			result.AddConstruct((StorageStatInfo){
				.file_name               = Move(platform_files[index].file_name),
				.size_in_bytes           = platform_files[index].size_in_bytes,
				.unix_time_created       = platform_files[index].unix_time_created,
				.unix_time_last_accessed = platform_files[index].unix_time_last_accessed,
				.unix_time_last_modified = platform_files[index].unix_time_last_modified,
			});
		}
		return result;
	}

	Optional<IOError> LocalStorageProvider::EnsureDirectoryStructure(const StringView path) noexcept
	{
		if (path.IsEmpty()) [[unlikely]] {
			return k_null_option;
		}

		StringView::SizeType directory_index_end = 0UL;
		while (true) {
			directory_index_end = path.Find(StringView("/"), directory_index_end);
			if (directory_index_end == StringView::k_invalid_index) {
				return k_null_option;
			}

			const StringView directory_path = path.SubView(0UL, ++directory_index_end);
			if (!DirectoryExists(directory_path)) {
				if (!DirectoryCreate(directory_path)) {
					return IOError::DirectoryCreationFailed;
				}
			}
		}
	}
}  // namespace Orion::Engine::Platform::FileSystem
