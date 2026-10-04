#pragma once

#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"
#include "Platform/FileSystem/StorageProvider.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief LocalStorageProvider represents an access point into the underlying local storage of a platform the
	/// engine runs on. In reality, it's a rather thin abstraction over bunch of Platform's IO calls.
	/// @details Prefer it over raw Platform's IO calls.
	class LocalStorageProvider final : public IStorageProvider
	{
		public:
		using ThisType = LocalStorageProvider;

		public:
		explicit LocalStorageProvider() noexcept = default;
		~LocalStorageProvider() override         = default;

		[[nodiscard]] static StringView Protocol() noexcept;

		[[nodiscard]] Optional<IOError> Create(StringView path) noexcept override;
		[[nodiscard]] Optional<IOError> Remove(StringView path) noexcept override;
		[[nodiscard]] IOResult<IStorageFileWriter*> Write(StringView path) noexcept override;
		[[nodiscard]] IOResult<IStorageFileReader*> Read(StringView path) noexcept override;
		[[nodiscard]] IOResult<StorageStatInfo> Stat(StringView path) noexcept override;
		[[nodiscard]] Vector<StorageStatInfo> List(StringView path, StorageListOption list_option) noexcept override;

		private:
		[[nodiscard]] static Optional<IOError> EnsureDirectoryStructure(StringView path) noexcept;
	};

	/// @brief TODO
	class LocalStorageFileWriter final : public IStorageFileWriter
	{
		public:
		explicit LocalStorageFileWriter(StringView path);
		~LocalStorageFileWriter() override = default;

		private:
	};

	/// @brief TODO
	class LocalStorageFileReader final : public IStorageFileReader
	{
		public:
		~LocalStorageFileReader() override = default;
	};

}  // namespace Orion::Engine::Platform::FileSystem
