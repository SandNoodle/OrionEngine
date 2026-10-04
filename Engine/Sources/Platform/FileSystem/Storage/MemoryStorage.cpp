#include "Platform/FileSystem/Storage/MemoryStorage.h"

namespace Orion::Engine::Platform::FileSystem
{
	StringView MemoryStorageProvider::Protocol() noexcept
	{
		return StringView("memory://");
	}

	Optional<IOError> MemoryStorageProvider::Create(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Optional<IOError> MemoryStorageProvider::Remove(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	IOResult<IStorageFileWriter*> MemoryStorageProvider::Write(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	IOResult<IStorageFileReader*> MemoryStorageProvider::Read(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	IOResult<StorageStatInfo> MemoryStorageProvider::Stat(StringView path) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	Vector<StorageStatInfo> MemoryStorageProvider::List(StringView path, StorageListOption list_option) noexcept
	{
		ORION_IGNORE_PARAM(path);
		ORION_IGNORE_PARAM(list_option);
		ORION_NOT_IMPLEMENTED();
	}
}  // namespace Orion::Engine::Platform::FileSystem
