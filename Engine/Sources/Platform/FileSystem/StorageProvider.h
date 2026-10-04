#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/Result.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief Enumeration describing type of an error encountered when performing Storage's IO operations.
	enum class IOError : UInt8
	{
		FileCreationFailed,
		FileDeletionFailed,
		FileDoesNotExist,

		DirectoryCreationFailed,
		DirectoryDeletionFailed,

		ProtocolNotPresent,
		ProtocolNotRecognized,

		InternalError,
	};

	/// @brief Result type for IO operations.
	/// @tparam T Expected type to be stored.
	template <typename T>
	using IOResult = Result<T, IOError>;

	/// @brief Structure tha holds metadata of a given file in the storage.
	struct StorageStatInfo
	{
		String file_name;
		UInt64 size_in_bytes;
		UInt64 unix_time_created;
		UInt64 unix_time_last_accessed;
		UInt64 unix_time_last_modified;
	};

	/// @brief Enumeration describing what kind of behavior should be used when listing files from a given path.
	enum class StorageListOption : Bool8
	{
		NonRecursive = false,
		Recursive    = true,
	};

	/// @brief Represents writing access point to the underlying file, be it local, in-memory, etc.
	/// @warning Only one access operation can be active for a given file. If the file is locked for reading then no
	/// writing can take place. However, multiple writers CANNOT access the same file concurrently.
	class IStorageFileWriter
	{
		public:
		virtual ~IStorageFileWriter() = default;
	};

	/// @brief Represents reading access point to the underlying file, be it local, in-memory, etc.
	/// @warning Only one access operation can be active for a given file. If the file is locked for reading
	/// then no writing can take place. However, multiple multiple readers CAN access the same file concurrently.
	class IStorageFileReader
	{
		public:
		virtual ~IStorageFileReader() = default;
	};

	/// @brief Represents an access point into the underlying storage, be it local, in-memory, etc.
	/// @warning StorageProviders have no concept of 'directories', i.e. they treat files with their path as a single
	/// (albeit long) filenames.
	class IStorageProvider
	{
		public:
		virtual ~IStorageProvider() = default;

		/// @brief Attempts to create a file under a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to create.
		[[nodiscard]] virtual Optional<IOError> Create(StringView path) noexcept = 0;

		/// @brief Attempts to remove a file under a given \p path.
		/// @warning Removing non-existent file doesn't result in an Error.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to remove.
		[[nodiscard]] virtual Optional<IOError> Remove(StringView path) noexcept = 0;

		/// @brief Attempts to provide a writer to a file at a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to write.
		[[nodiscard]] virtual IOResult<IStorageFileWriter*> Write(StringView path) noexcept = 0;

		/// @brief Attempts to provide a reader of a file at a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to read.
		[[nodiscard]] virtual IOResult<IStorageFileReader*> Read(StringView path) noexcept = 0;

		/// @brief Queries the underlying storage to check that file exists under a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the file to stat.
		[[nodiscard]] virtual IOResult<StorageStatInfo> Stat(StringView path) noexcept = 0;

		/// @brief Queries the underlying storage to list every file under a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param[IN, REQUIRED] path Path to the 'directory' under which to query the files.
		/// @param[IN, REQUIRED] list_option What kind of behavior should be used when iterating over \p path.
		[[nodiscard]] virtual Vector<StorageStatInfo> List(StringView path, StorageListOption list_option) noexcept = 0;
	};
}  // namespace Orion::Engine::Platform::FileSystem
