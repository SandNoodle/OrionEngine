#include "Platform/FileSystem/FileSystem.h"

#include "Core/Assert.h"
#include "Core/Log/Logger.h"
#include "Platform/FileSystem/Storage/LocalStorage.h"
#include "Platform/FileSystem/Storage/MemoryStorage.h"

namespace Orion::Engine::Platform::FileSystem
{
	Bool8 FileSystem::Initialize() noexcept
	{
		ORION_LOG_DEBUG("[FileSystem] Initializing...");
		Bool8 is_initialized = true;

		IStorageProvider* local_storage_provider  = Memory::AllocateConstruct<LocalStorageProvider>(_allocator);
		IStorageProvider* memory_storage_provider = Memory::AllocateConstruct<MemoryStorageProvider>(_allocator);

		is_initialized &= RegisterStorageProvider(LocalStorageProvider::Protocol(), local_storage_provider);
		is_initialized &= RegisterStorageProvider(MemoryStorageProvider::Protocol(), memory_storage_provider);

		ORION_LOG_DEBUG("[FileSystem] Initialized.");
		return is_initialized;
	}

	Bool8 FileSystem::Shutdown() noexcept
	{
		ORION_LOG_DEBUG("[FileSystem] Shutting down.");
		for (auto&& [_, provider] : _storage_providers) {
			DestroyProvider(provider);
		}
		_storage_providers.Clear();
		ORION_LOG_DEBUG("[FileSystem] Shut down.");
		return true;
	}

	Optional<IOError> FileSystem::Create(StringView path) noexcept
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Create(path_without_prefix);
	}

	Optional<IOError> FileSystem::Remove(StringView path) noexcept
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Remove(path_without_prefix);
	}

	IOResult<IStorageFileWriter*> FileSystem::Write(StringView path) noexcept
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Write(path_without_prefix);
	}

	IOResult<IStorageFileReader*> FileSystem::Read(StringView path) noexcept
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Read(path_without_prefix);
	}

	IOResult<StorageStatInfo> FileSystem::Stat(StringView path) noexcept
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Stat(path_without_prefix);
	}

	Vector<StorageStatInfo> FileSystem::List(StringView path, StorageListOption list_option) noexcept
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return Vector<StorageStatInfo>{};
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->List(path_without_prefix, list_option);
	}

	void FileSystem::DestroyProvider(IStorageProvider* storage_provider) noexcept
	{
		if (!storage_provider) {
			ORION_LOG_WARN("[FileSystem] Attempting to destroy invalid provider (nullptr).");
			return;
		}
		Memory::FreeDestruct(_allocator, storage_provider);
		storage_provider = nullptr;
	}

	Bool8 FileSystem::RegisterStorageProvider(StringView protocol, IStorageProvider* storage_provider) noexcept
	{
		if (!storage_provider) {
			ORION_LOG_ERROR("[FileSystem] Failed to initialize StorageProvider for protocol '{}' (nullptr).", protocol);
			return false;
		}

		// NOTE: We cannot register multiple StorageProviders operating on the same protocol as they will conflict
		//       with each other - and there isn't much point nor meaning in trying to resolve this. Instead we just
		//       don't register the provider at all.
		if (_storage_providers.Contains(protocol)) {
			ORION_LOG_ERROR("[FileSystem] StorageProvider for '{}' protocol already exists.", protocol);
			return false;
		}

		ORION_LOG_DEBUG("[FileSystem] Initialized StorageProvider for '{}' protocol.", protocol);
		_storage_providers.Insert(protocol, storage_provider);

		return true;
	}

	IOResult<Pair<IStorageProvider*, StringView>> FileSystem::GetProviderAndPath(StringView path) noexcept
	{
		USize split_index = path.Find(StringView("://"));
		if (split_index == StringView::k_invalid_index) {
			ORION_LOG_ERROR("[FileSystem] Failed to stat file ('{}'), protocol is not present.", path);
			return IOError::ProtocolNotPresent;
		}

		split_index += 3;  // Skip over the separator (protocol's suffix).
		StringView protocol_name       = path.SubView(0, split_index);
		StringView path_without_prefix = path.SubView(split_index, path.Size());
		if (!_storage_providers.Contains(protocol_name)) {
			ORION_LOG_ERROR(
				"[FileSystem] Failed to stat file ('{}'), unrecognized protocol: '{}'.", path, protocol_name);
			return IOError::ProtocolNotRecognized;
		}

		IStorageProvider* storage_provider = _storage_providers[protocol_name];
		if (!storage_provider) {
			ORION_LOG_ERROR(
				"[FileSystem] Failed to stat file ('{}'), StorageProvider for '{}' protocol does not exist.",
				path,
				protocol_name);
			return IOError::InternalError;
		}
		return Pair(storage_provider, path_without_prefix);
	}
}  // namespace Orion::Engine::Platform::FileSystem
