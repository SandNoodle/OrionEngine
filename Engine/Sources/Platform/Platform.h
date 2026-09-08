#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/EnumFlag.h"

namespace Orion::Engine::Platform
{
	/// @brief Structure holding capabilities of the underlying platform.
	struct PlatformInfo
	{
		StringView system_name;
		UInt64 page_size_in_bytes;
		UInt64 large_page_size_in_bytes;
		UInt32 logical_processor_count;
	};

	/// @brief Enumeration describing access type of the platform's file.
	enum class PlatformFileAccessFlags : UInt8
	{
		None  = 0x0,
		Read  = 0x1,
		Write = 0x2,

		All = Read | Write,
	};
	ORION_ENUM_FLAG(PlatformFileAccessFlags);

	/// @brief Structure holding metadata of the platform's file.
	struct PlatformFileStat
	{
		String file_name;
		UInt64 size_in_bytes;
		UInt64 unix_time_created;
		UInt64 unix_time_last_modified;
		UInt64 unix_time_last_accessed;
		PlatformFileAccessFlags access_flags;
	};

	/// @brief Queries and returns capabilities of the underlying platform.
	[[nodiscard]] PlatformInfo GetPlatformInfo() noexcept;

	/// @brief Queries the underlying platform and creates new file under a given \p path.
	/// @param[IN, REQUIRED] path Path to the file.
	/// @param[IN, REQUIRED] flags Access flags to the new file, i.e. should the file be readable, writable, etc.
	/// @warning Will not override existing files.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 FileCreate(StringView path, PlatformFileAccessFlags flags) noexcept;

	/// @brief Queries the underlying platform and removes file under a given \p path.
	/// @param[IN, REQUIRED] path Path to the file.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 FileRemove(StringView path) noexcept;

	/// @brief Queries the underlying platform to check that file exists under a given path.
	/// @param[IN, REQUIRED] path Path to the file.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 FileExists(StringView path) noexcept;

	/// @brief Queries the underlying platform to acquire metadata about the file under a given path.
	/// @warning Assumes that FileExists(...) was called beforehand, i.e. file exists.
	/// @param[IN, REQUIRED] path Path to the file.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] PlatformFileStat StatFile(StringView path) noexcept;

	/// @brief Queries the underlying platform and stats the list of file under a given \p path.
	/// @param[IN, REQUIRED] path Path to the directory.
	/// @param[IN, REQUIRED] recursive Should sub-directories be traversed?
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Vector<PlatformFileStat> ListFiles(StringView path, Bool8 recursive) noexcept;

	/// @brief Queries the underlying platform and creates a new directory under a given \p path.
	/// @param[IN, REQUIRED] path Path to the directory to be created.
	/// @warning Will not create directories recursively!
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 DirectoryCreate(StringView path) noexcept;

	/// @brief Queries the underlying platform and removes directory under a given \p path.
	/// @param[IN, REQUIRED] path Path to the directory to be removed .
	/// @warning Will not remove directories recursively!
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 DirectoryRemove(StringView path) noexcept;

	/// @brief Queries the underlying platform to check that directory exists under a given path.
	/// @param[IN, REQUIRED] path Path to the directory.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 DirectoryExists(StringView path) noexcept;
}  // namespace Orion::Engine::Platform
