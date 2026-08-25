#pragma once

#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/Result.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Memory/Allocators/Allocator.h"
#include "Core/Standard/Utility/MoveAndForward.h"
#include "Platform/FileSystem/StorageProvider.h"
#include "Platform/Platform.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief TODO
	template <Memory::AllocatorKind Allocator>
	class LocalStorageProvider final : public IStorageProvider
	{
		public:
		using ThisType      = LocalStorageProvider;
		using AllocatorType = Allocator;

		private:
		AllocatorType _allocator;

		public:
		constexpr explicit LocalStorageProvider(const AllocatorType& allocator = AllocatorType()) noexcept;
		~LocalStorageProvider() override = default;

		/// @brief TODO
		/// @param[IN, REQUIRED] allocator TODO
		[[nodiscard]] static constexpr ThisType* Create(AllocatorType& allocator = AllocatorType()) noexcept;

		[[nodiscard]] StorageProviderProtocol Protocol() noexcept override;
		[[nodiscard]] Optional<IOError> Create(StringView path) noexcept override;
		[[nodiscard]] Optional<IOError> Remove(StringView path) noexcept override;
		[[nodiscard]] IOResult<IStorageFileWriter*> Write(StringView path) noexcept override;
		[[nodiscard]] IOResult<IStorageFileReader*> Read(StringView path) noexcept override;
		[[nodiscard]] IOResult<StorageStatInfo> Stat(StringView path) noexcept override;
		[[nodiscard]] Vector<StorageStatInfo> List(StringView path, Bool8 recursive) noexcept override;

		private:
		[[nodiscard]] static constexpr String SanitizeIntoBuffer(StringView path) noexcept;
	};

	/// @brief TODO
	template <Memory::AllocatorKind Allocator>
	class LocalStorageFileWriter final : public IStorageFileWriter
	{
		public:
		~LocalStorageFileWriter() override = default;
	};

	/// @brief TODO
	template <Memory::AllocatorKind Allocator>
	class LocalStorageFileReader final : public IStorageFileReader
	{
		public:
		~LocalStorageFileReader() override = default;
	};

	// -- Implementation.
	template <Memory::AllocatorKind Allocator>
	constexpr LocalStorageProvider<Allocator>::LocalStorageProvider(const AllocatorType& allocator) noexcept
		: _allocator(allocator)
	{
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto LocalStorageProvider<Allocator>::Create(AllocatorType& allocator) noexcept -> ThisType*
	{
		ThisType* provider = static_cast<ThisType*>(allocator.Allocate(sizeof(ThisType), alignof(ThisType)));
		if (provider) {
			Memory::ConstructItem(provider, allocator);
		}
		return provider;
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::Protocol() noexcept -> StorageProviderProtocol
	{
		return StorageProviderProtocol::Local;
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::Create(StringView path) noexcept -> Optional<IOError>
	{
		String buffer     = SanitizeIntoBuffer(path);
		CString file_path = reinterpret_cast<CString>(buffer.Data());
		if (!FileCreate(file_path, PlatformFileAccessFlags::All)) {
			return IOError::FileCreationFailed;
		}
		return k_null_option;
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::Remove(StringView path) noexcept -> Optional<IOError>
	{
		String buffer     = SanitizeIntoBuffer(path);
		CString file_path = reinterpret_cast<CString>(buffer.Data());
		if (!FileRemove(file_path)) {
			return IOError::FileDeletionFailed;
		}
		return k_null_option;
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::Write(StringView path) noexcept -> IOResult<IStorageFileWriter*>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::Read(StringView path) noexcept -> IOResult<IStorageFileReader*>
	{
		ORION_IGNORE_PARAM(path);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::Stat(StringView path) noexcept -> IOResult<StorageStatInfo>
	{
		String buffer     = SanitizeIntoBuffer(path);
		CString file_path = reinterpret_cast<CString>(buffer.Data());

		if (!FileExists(file_path)) {
			return IOError::FileDoesNotExist;
		}

		PlatformFileStat platform_file_stat = StatFile(file_path);
		return (StorageStatInfo){
			.file_name          = Move(platform_file_stat.file_name),
			.size_in_bytes      = platform_file_stat.size_in_bytes,
			.time_created       = platform_file_stat.time_created,
			.time_last_accessed = platform_file_stat.time_last_accessed,
			.time_last_modified = platform_file_stat.time_last_modified,
		};
	}

	template <Memory::AllocatorKind Allocator>
	auto LocalStorageProvider<Allocator>::List(StringView path, Bool8 recursive) noexcept -> Vector<StorageStatInfo>
	{
		ORION_IGNORE_PARAM(path);
		ORION_IGNORE_PARAM(recursive);
		ORION_NOT_IMPLEMENTED();
	}

	template <Memory::AllocatorKind Allocator>
	constexpr auto LocalStorageProvider<Allocator>::SanitizeIntoBuffer(StringView path) noexcept -> String
	{
		String buffer{};
		buffer.Reserve(path.Size() + 1);
		buffer.AppendRange(path.begin(), path.end());
		buffer.Append(static_cast<String::WideCharType>('\0'));  // Null terminator.
		return buffer;
	}
}  // namespace Orion::Engine::Platform::FileSystem
