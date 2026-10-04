#pragma once

#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"
#include "Platform/FileSystem/StorageProvider.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief MemoryStorageProvider represents an access point into storage made entirely in the engine's runtime
	/// memory. In reality, it's just an allocated chunk of memory where 'files' live.
	/// @details Prefer it for short-lived temporary files.
	class MemoryStorageProvider final : public IStorageProvider
	{
		public:
		using ThisType = MemoryStorageProvider;

		public:
		explicit MemoryStorageProvider() noexcept = default;
		~MemoryStorageProvider() override         = default;

		[[nodiscard]] static StringView Protocol() noexcept;

		[[nodiscard]] Optional<IOError> Create(StringView path) noexcept override;
		[[nodiscard]] Optional<IOError> Remove(StringView path) noexcept override;
		[[nodiscard]] IOResult<IStorageFileWriter*> Write(StringView path) noexcept override;
		[[nodiscard]] IOResult<IStorageFileReader*> Read(StringView path) noexcept override;
		[[nodiscard]] IOResult<StorageStatInfo> Stat(StringView path) noexcept override;
		[[nodiscard]] Vector<StorageStatInfo> List(StringView path, StorageListOption list_option) noexcept override;
	};

	/// @brief TODO
	class MemoryStorageFileWriter : public IStorageFileWriter
	{
		public:
		~MemoryStorageFileWriter() override = default;
	};

	/// @brief TODO
	class MemoryStorageFileReader final : public IStorageFileReader
	{
		public:
		~MemoryStorageFileReader() override = default;
	};
}  // namespace Orion::Engine::Platform::FileSystem
