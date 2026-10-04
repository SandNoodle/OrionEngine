#pragma once

#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/Pair.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/Memory/Allocators/PlatformAllocator.h"
#include "Platform/FileSystem/StorageProvider.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief FileSystem is a system responsible for any and all IO file operations in the engine.
	class FileSystem final
	{
		public:
		using ThisType = FileSystem;

		private:
		Memory::PlatformAllocator _allocator{};
		HashMap<StringView, IStorageProvider*> _storage_providers{};

		public:
		FileSystem()                         = default;
		FileSystem(const ThisType&) noexcept = delete;
		FileSystem(ThisType&&) noexcept      = delete;
		~FileSystem()                        = default;

		FileSystem& operator=(const ThisType&) noexcept = delete;
		FileSystem& operator=(ThisType&&) noexcept      = delete;

		/// @brief Initializes the filesystem and prepares it for further use. Initializes StorageProviders and their
		/// protocols.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Initialize() noexcept;

		/// @brief Shutdowns the filesystem. Deinitializes all StorageProviders.
		/// @warning Must be called exactly once!
		[[nodiscard]] Bool8 Shutdown() noexcept;

		/// @brief Attempts to create a file under a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to create.
		[[nodiscard]] Optional<IOError> Create(StringView path) noexcept;

		/// @brief Attempts to remove a file under a given \p path.
		/// @warning Removing non-existent file doesn't result in an Error.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to remove.
		[[nodiscard]] Optional<IOError> Remove(StringView path) noexcept;

		/// @brief Attempts to provide a writer to a file at a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to write.
		[[nodiscard]] IOResult<IStorageFileWriter*> Write(StringView path) noexcept;

		/// @brief Attempts to provide a reader of a file at a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to read.
		[[nodiscard]] IOResult<IStorageFileReader*> Read(StringView path) noexcept;

		/// @brief Queries the filesystem to check that the file exists under a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to stat.
		[[nodiscard]] IOResult<StorageStatInfo> Stat(StringView path) noexcept;

		/// @brief Queries the filesystem to list every file under a given \p path.
		/// @warning \p path MUST contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the 'directory' under which to query the files.
		/// @param[IN, REQUIRED] list_option What kind of behavior should be used when iterating over \p path.
		[[nodiscard]] Vector<StorageStatInfo> List(StringView path, StorageListOption list_option) noexcept;

		private:
		void DestroyProvider(IStorageProvider* storage_provider) noexcept;
		[[nodiscard]] Bool8 RegisterStorageProvider(StringView protocol, IStorageProvider* storage_provider) noexcept;
		[[nodiscard]] IOResult<Pair<IStorageProvider*, StringView>> GetProviderAndPath(StringView path) noexcept;
	};
}  // namespace Orion::Engine::Platform::FileSystem
