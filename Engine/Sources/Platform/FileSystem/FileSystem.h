#pragma once

#include "Core/Assert.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Memory/Allocators/Allocator.h"
#include "Core/Standard/Memory/Allocators/PlatformAllocator.h"
#include "Platform/FileSystem/Storage/LocalStorage.h"
#include "Platform/FileSystem/Storage/MemoryStorage.h"
#include "StorageProvider.h"

namespace Orion::Engine::Platform::FileSystem
{
	/**
	 * @brief TODO
	 * @tparam Allocator
	 */
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

		private:
		/// @brief TODO
		[[nodiscard]] constexpr Bool8 RegisterStorageProvider(StorageProviderProtocol protocol,
		                                                      IStorageProvider* storage_provider) noexcept;
	};

	// -- Implementation.
	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Initialize() noexcept -> Bool8
	{
		Bool8 is_initialized = true;

		LocalStorageProvider<AllocatorType>* local_storage_provider
			= LocalStorageProvider<AllocatorType>::Create(_allocator);
		MemoryStorageProvider<AllocatorType>* memory_storage_provider
			= MemoryStorageProvider<AllocatorType>::Create(_allocator);

		is_initialized &= RegisterStorageProvider(StorageProviderProtocol::Local, local_storage_provider);
		is_initialized &= RegisterStorageProvider(StorageProviderProtocol::Memory, memory_storage_provider);

		return is_initialized;
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::Shutdown() noexcept -> Bool8
	{
		_storage_providers.Clear();
		return true;
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto FileSystem<Allocator>::RegisterStorageProvider(StorageProviderProtocol protocol,
	                                                              IStorageProvider* storage_provider) noexcept -> Bool8
	{
		if (!storage_provider) {
			ORION_LOG_ERROR("[FileSystem] Failed to initialize StorageProvider (nullptr).");
			return false;
		}

		// NOTE: We cannot register multiple StorageProviders operating on the same protocol as they will conflict with
		//       each other - and there isn't much point nor meaning in trying to resolve this. Instead we just don't
		//       register the provider at all.
		if (_storage_providers.Contains(protocol)) {
			ORION_LOG_ERROR("[FileSystem] StorageProvider for '{}' protocol already exists.", ProtocolName(protocol));
			return false;
		}

		ORION_LOG_DEBUG("[FileSystem] Initialized StorageProvider for '{}' protocol.", ProtocolName(protocol));
		_storage_providers.Insert(protocol, storage_provider);

		return true;
	}
}  // namespace Orion::Engine::Platform::FileSystem
