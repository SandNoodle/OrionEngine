#pragma once

#include "Platform/FileSystem/StorageProvider.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief TODO
	template <Memory::AllocatorKind Allocator>
	class MemoryStorageProvider final : public IStorageProvider
	{
		public:
		using ThisType      = MemoryStorageProvider;
		using AllocatorType = Allocator;

		private:
		AllocatorType _allocator;

		public:
		constexpr explicit MemoryStorageProvider(const AllocatorType& allocator = AllocatorType()) noexcept;
		~MemoryStorageProvider() override = default;

		/// @brief TODO
		/// @param[IN, REQUIRED] allocator TODO
		[[nodiscard]] static constexpr ThisType* Create(AllocatorType& allocator = AllocatorType()) noexcept;

		[[nodiscard]] StorageProviderProtocol Protocol() noexcept override;
		[[nodiscard]] Optional<Error> Create(StringView path) noexcept override;
		[[nodiscard]] Optional<Error> Remove(StringView path) noexcept override;
		[[nodiscard]] Result<IStorageFileWriter*> Write(StringView path) noexcept override;
		[[nodiscard]] Result<IStorageFileReader*> Read(StringView path) noexcept override;
		[[nodiscard]] Result<StorageStatInfo> Stat(StringView path) noexcept override;
		[[nodiscard]] Vector<StorageStatInfo> List(StringView path, Bool8 recursive) noexcept override;
	};

	/// @brief TODO
	template <Memory::AllocatorKind Allocator>
	class MemoryStorageFileWriter : public IStorageFileWriter
	{
		public:
		using ThisType      = MemoryStorageFileWriter;
		using AllocatorType = Allocator;

		public:
		~MemoryStorageFileWriter() override = default;
	};

	/// @brief TODO
	template <Memory::AllocatorKind Allocator>
	class MemoryStorageFileReader final : public IStorageFileReader
	{
		public:
		using ThisType      = MemoryStorageFileReader;
		using AllocatorType = Allocator;

		public:
		~MemoryStorageFileReader() override = default;
	};

	// -- Implementation.
	template <Memory::AllocatorKind Allocator>
	constexpr MemoryStorageProvider<Allocator>::MemoryStorageProvider(const AllocatorType& allocator) noexcept
		: _allocator(allocator)
	{
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto MemoryStorageProvider<Allocator>::Create(AllocatorType& allocator) noexcept -> ThisType*
	{
		ThisType* provider = static_cast<ThisType*>(allocator.Allocate(sizeof(ThisType), alignof(ThisType)));
		if (provider) {
			Memory::ConstructItem(provider, allocator);
		}
		return provider;
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::Protocol() noexcept -> StorageProviderProtocol
	{
		return StorageProviderProtocol::Memory;
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::Create(StringView path) noexcept -> Optional<Error>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::Remove(StringView path) noexcept -> Optional<Error>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::Write(StringView path) noexcept -> Result<IStorageFileWriter*>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::Read(StringView path) noexcept -> Result<IStorageFileReader*>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::Stat(StringView path) noexcept -> Result<StorageStatInfo>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto MemoryStorageProvider<Allocator>::List(StringView path, Bool8 recursive) noexcept -> Vector<StorageStatInfo>
	{
		ORION_IGNORE_PARAM(path);
		ORION_IGNORE_PARAM(recursive);
		ORION_NOT_IMPLEMENTED();
	}
}  // namespace Orion::Engine::Platform::FileSystem
