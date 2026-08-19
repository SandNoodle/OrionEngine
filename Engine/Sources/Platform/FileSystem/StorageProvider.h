#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/Optional.h"
#include "Core/Standard/Containers/Result.h"
#include "Core/Standard/Containers/Span.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"

namespace Orion::Engine::Platform::FileSystem
{
	/// @brief TODO
	enum class StorageProviderProtocol : UInt8
	{
		Local,
		Memory,
	};

	/// @brief TODO
	struct StorageStatInfo
	{
		String file_name;
		UInt64 size_in_bytes;
		UInt64 time_created;
		UInt64 time_last_accessed;
		UInt64 time_last_modified;
	};

	/// @brief TODO
	class IStorageFileWriter
	{
		public:
		virtual ~IStorageFileWriter() = default;

#if 0
		/// @brief TODO
		/// @param [IN, REQUIRED] data TODO
		[[nodiscard]] virtual Optional<Error> Write(ReadonlySpan<Byte> data) = 0;

		/// @brief TODO
		/// @param [IN, REQUIRED] offset TODO
		[[nodiscard]] virtual Optional<Error> Seek(USize offset);

		/// @brief TODO
		[[nodiscard]] virtual Optional<Error> Close() = 0;
#endif
	};

	/// @brief TODO
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

		/// @brief Returns the underlying protocol, i.e. storage type for this StorageProvider.
		[[nodiscard]] virtual StorageProviderProtocol Protocol() noexcept = 0;

		/// @brief Attempts to create a file under a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to create.
		[[nodiscard]] virtual Optional<Error> Create(StringView path) noexcept = 0;

		/// @brief Attempts to remove a file under a given \p path.
		/// @warning Removing non-existent file doesn't result in an Error.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to remove.
		[[nodiscard]] virtual Optional<Error> Remove(StringView path) noexcept = 0;

		/// @brief Attempts to provide a writer to a file at a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to write.
		[[nodiscard]] virtual Result<IStorageFileWriter*> Write(StringView path) noexcept = 0;

		/// @brief Attempts to provide a reader of a file at a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to read.
		[[nodiscard]] virtual Result<IStorageFileReader*> Read(StringView path) noexcept = 0;

		/// @brief Queries the underlying storage to check that file exists under a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the file to stat.
		[[nodiscard]] virtual Result<StorageStatInfo> Stat(StringView path) noexcept = 0;

		/// @brief Queries the underlying storage to list every file under a given \p path.
		/// @warning \p path must NOT contain the protocol's prefix.
		/// @param [IN, REQUIRED] path Path to the 'directory' under which to query the files.
		/// @param [IN, REQUIRED] recursive Should files in sub-directories also be listed.
		[[nodiscard]] virtual Vector<StorageStatInfo> List(StringView path, Bool8 recursive) noexcept = 0;
	};

	/// @brief Returns human-readable name of a given StorageProvider's \p protocol.
	[[nodiscard]] constexpr StringView ProtocolName(StorageProviderProtocol protocol) noexcept
	{
		switch (protocol) {
			case StorageProviderProtocol::Local:
				return ORION_STRINGVIEW("local");
			case StorageProviderProtocol::Memory:
				return ORION_STRINGVIEW("memory");
			default:
				ORION_NOT_IMPLEMENTED("unhandled protocol name");
		}
	}

	/// @brief Returns (path) prefix for a given StorageProvider's \p protocol.
	[[nodiscard]] constexpr StringView ProtocolPrefix(StorageProviderProtocol protocol) noexcept
	{
		switch (protocol) {
			case StorageProviderProtocol::Local:
				return ORION_STRINGVIEW("local://");
			case StorageProviderProtocol::Memory:
				return ORION_STRINGVIEW("mem://");
			default:
				ORION_NOT_IMPLEMENTED("unhandled protocol prefix");
		}
	}
}  // namespace Orion::Engine::Platform::FileSystem

template <>
struct Orion::Engine::Algorithm::Hash<Orion::Engine::Platform::FileSystem::StorageProviderProtocol>
{
	public:
	using ValueType = Platform::FileSystem::StorageProviderProtocol;
	using SizeType  = USize;

	public:
	SizeType operator()(const ValueType& v) const
	{
		return static_cast<SizeType>(v);
	}
};  // namespace Orion::Engine::Algorithm
