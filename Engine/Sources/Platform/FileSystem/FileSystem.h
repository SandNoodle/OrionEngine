#pragma once

#include "Core/Assert.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/Pair.h"
#include "Core/Standard/Containers/Result.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Memory/Allocators/Allocator.h"
#include "Core/Standard/Memory/Allocators/PlatformAllocator.h"
#include "Platform/FileSystem/Storage/LocalStorage.h"
#include "Platform/FileSystem/Storage/MemoryStorage.h"
#include "StorageProvider.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief TODO
	/// @tparam Allocator TODO
	template <Memory::AllocatorKind Allocator = Memory::PlatformAllocator>
	class FileSystem final
	{
		public:
		using ThisType      = FileSystem;
		using AllocatorType = Allocator;

		private:
		using StorageProviders = HashMap<StorageProviderProtocol,
		                                 IStorageProvider*,
		                                 Algorithm::Hash<StorageProviderProtocol>,
		                                 Algorithm::Equal<StorageProviderProtocol>,
		                                 AllocatorType>;

		private:
		AllocatorType _allocator{};
		StorageProviders _storage_providers{};

		public:
		constexpr FileSystem()                         = default;
		constexpr FileSystem(const ThisType&) noexcept = delete;
		constexpr FileSystem(ThisType&&) noexcept      = delete;
		constexpr ~FileSystem()                        = default;

		constexpr FileSystem& operator=(const ThisType&) noexcept = delete;
		constexpr FileSystem& operator=(ThisType&&) noexcept      = delete;

		/// @brief TODO
		[[nodiscard]] constexpr Bool8 Initialize() noexcept;

		/// @brief TODO
		[[nodiscard]] constexpr Bool8 Shutdown() noexcept;

		/// @brief Attempts to create a file under a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to create.
		[[nodiscard]] constexpr Optional<IOError> Create(StringView path) noexcept;

		/// @brief Attempts to remove a file under a given \p path.
		/// @warning Removing non-existent file doesn't result in an Error.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to remove.
		[[nodiscard]] constexpr Optional<IOError> Remove(StringView path) noexcept;

		/// @brief Attempts to provide a writer to a file at a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to write.
		[[nodiscard]] constexpr IOResult<IStorageFileWriter*> Write(StringView path) noexcept;

		/// @brief Attempts to provide a reader of a file at a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to read.
		[[nodiscard]] constexpr IOResult<IStorageFileReader*> Read(StringView path) noexcept;

		/// @brief Queries the filesystem to check that the file exists under a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to stat.
		[[nodiscard]] constexpr IOResult<StorageStatInfo> Stat(StringView path) noexcept;

		/// @brief Queries the filesystem to list every file under a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the 'directory' under which to query the files.
		/// @param [IN, REQUIRED] recursive Should files in sub-directories also be listed.
		[[nodiscard]] constexpr Vector<StorageStatInfo> List(StringView path, Bool8 recursive) noexcept;

		private:
		/// @brief TODO
		[[nodiscard]] constexpr Bool8 RegisterStorageProvider(StorageProviderProtocol protocol,
		                                                      IStorageProvider* storage_provider) noexcept;

		/// @brief TODO
		[[nodiscard]] constexpr IOResult<Pair<IStorageProvider*, StringView>> GetProviderAndPath(
			StringView path) noexcept;
	};

	// -- Implementation.
	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Initialize() noexcept -> Bool8
	{
		ORION_LOG_DEBUG("[FileSystem] Initializing...");
		Bool8 is_initialized = true;

		LocalStorageProvider<AllocatorType>* local_storage_provider
			= LocalStorageProvider<AllocatorType>::Create(_allocator);
		MemoryStorageProvider<AllocatorType>* memory_storage_provider
			= MemoryStorageProvider<AllocatorType>::Create(_allocator);

		is_initialized &= RegisterStorageProvider(StorageProviderProtocol::Local, local_storage_provider);
		is_initialized &= RegisterStorageProvider(StorageProviderProtocol::Memory, memory_storage_provider);

		ORION_LOG_DEBUG("[FileSystem] Initialized.");
		return is_initialized;
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Shutdown() noexcept -> Bool8
	{
		ORION_LOG_DEBUG("[FileSystem] Shutting down.");
		_storage_providers.Clear();
		return true;
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Create(StringView path) noexcept -> Optional<IOError>
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Create(path_without_prefix);
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Remove(StringView path) noexcept -> Optional<IOError>
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Remove(path_without_prefix);
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Write(StringView path) noexcept -> IOResult<IStorageFileWriter*>
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Write(path_without_prefix);
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Read(StringView path) noexcept -> IOResult<IStorageFileReader*>
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Read(path_without_prefix);
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Stat(StringView path) noexcept -> IOResult<StorageStatInfo>
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return result.Error();
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->Stat(path_without_prefix);
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::List(StringView path, Bool8 recursive) noexcept -> Vector<StorageStatInfo>
	{
		IOResult<Pair<IStorageProvider*, StringView>> result = GetProviderAndPath(path);
		if (result.IsError()) {
			return Vector<StorageStatInfo>{};
		}
		IStorageProvider* storage_provider = result.Value().first;
		StringView path_without_prefix     = result.Value().second;
		return storage_provider->List(path_without_prefix, recursive);
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::RegisterStorageProvider(StorageProviderProtocol protocol,
	                                                              IStorageProvider* storage_provider) noexcept -> Bool8
	{
		if (!storage_provider) {
			ORION_LOG_ERROR("[FileSystem] Failed to initialize StorageProvider (nullptr).");
			return false;
		}

		// NOTE: We cannot register multiple StorageProviders operating on the same protocol as they will conflict
		// with
		//       each other - and there isn't much point nor meaning in trying to resolve this. Instead we just
		//       don't register the provider at all.
		if (_storage_providers.Contains(protocol)) {
			ORION_LOG_ERROR("[FileSystem] StorageProvider for '{}' protocol already exists.", ProtocolName(protocol));
			return false;
		}

		ORION_LOG_DEBUG("[FileSystem] Initialized StorageProvider for '{}' protocol.", ProtocolName(protocol));
		_storage_providers.Insert(protocol, storage_provider);

		return true;
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::GetProviderAndPath(StringView path) noexcept
		-> IOResult<Pair<IStorageProvider*, StringView>>
	{
		USize split_index = path.Find(ORION_STRINGVIEW("://"));
		if (split_index == StringView::k_invalid_index) {
			ORION_LOG_ERROR("[FileSystem] Failed to stat file ('{}'), protocol is not present.", path);
			return IOError::ProtocolNotPresent;
		}

		split_index += 3;  // Skip over the separator (protocol's suffix).
		StringView protocol_name                          = path.SubView(0, split_index);
		StringView path_without_prefix                    = path.SubView(split_index, path.Size());
		Optional<StorageProviderProtocol> protocol_result = FromProtocolPrefix(protocol_name);
		if (!protocol_result.IsValue()) {
			ORION_LOG_ERROR(
				"[FileSystem] Failed to stat file ('{}'), unrecognized protocol: '{}'.", path, protocol_name);
			return IOError::ProtocolNotRecognized;
		}

		IStorageProvider* storage_provider = _storage_providers[*protocol_result];
		if (!storage_provider) {
			ORION_LOG_ERROR(
				"[FileSystem] Failed to stat file ('{}'), StorageProvider for '{}' protocol does not exist.",
				path,
				ProtocolName(*protocol_result));
			return IOError::InternalError;
		}
		return Pair(storage_provider, path_without_prefix);
	}
}  // namespace Orion::Engine::Platform::FileSystem
