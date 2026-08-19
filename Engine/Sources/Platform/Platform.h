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
		/// @brief Name of the underlying operating system, ex. Windows, Linux.
		StringView system_name;

		/// @brief Allocation size of a single memory page.
		UInt64 page_size;

		/// @brief Allocation size of a single large memory page.
		UInt64 large_page_size;

		/// @brief Number of logical processors that are available to the engine.
		UInt32 logical_processor_count;
	};

	/// @brief TODO
	enum class PlatformFileAccessFlags : UInt8
	{
		Read  = 0x0,
		Write = 0x1,

		All = Read | Write,
	};
	ORION_ENUM_FLAG(PlatformFileAccessFlags);

	/// @brief TODO
	struct PlatformFileStat
	{
		String file_name;
		UInt64 size_in_bytes;
		UInt64 time_created;
		UInt64 time_last_modified;
		UInt64 time_last_accessed;
		PlatformFileAccessFlags access_flags;
	};

	/// @brief Queries and returns capabilities of the underlying platform.
	[[nodiscard]] PlatformInfo GetPlatformInfo() noexcept;

	/// @brief TODO
	/// @param path TODO
	/// @param flags TODO
	[[nodiscard]] Bool8 FileCreate(CString path, PlatformFileAccessFlags flags) noexcept;

	/// @brief TODO
	/// @param path TODO
	[[nodiscard]] Bool8 FileRemove(CString path) noexcept;

	/// @brief Queries the underlying platform to check that file exists under a given path.
	/// @param path Null-terminated path to the file.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 FileExists(CString path) noexcept;

	/// @brief Queries the underlying platform to acquire metadata about the file under a given path.
	/// @warning Assumes that FileExists(...) was called beforehand, i.e. file exists.
	/// @param path Null-terminated path to the file.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] PlatformFileStat StatFile(CString path) noexcept;

	/// @brief TODO
	/// @param path TODO
	[[nodiscard]] Vector<PlatformFileStat> ListFiles(CString path) noexcept;

	/// @brief TODO
	/// @param path TODO
	[[nodiscard]] Bool8 DirectoryCreate(CString path) noexcept;

	/// @brief TODO
	/// @param path TODO
	[[nodiscard]] Bool8 DirectoryRemove(CString path) noexcept;

	/// @brief Queries the underlying platform to check that directory exists under a given path.
	/// @param path Null-terminated path to the directory.
	/// @warning DO NOT USE DIRECTLY! All filesystem calls should be handled through the Platform::FileSystem module.
	[[nodiscard]] Bool8 DirectoryExists(CString path) noexcept;
}  // namespace Orion::Engine::Platform
