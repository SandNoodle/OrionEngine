#include "TestCommon.h"

#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Platform/FileSystem/Storage/LocalStorage.h"

namespace Orion::Engine::Platform::FileSystem
{
	namespace
	{
		class LocalStorageTest : public ::testing::Test
		{
			protected:
			Memory::PlatformAllocator _allocator{};
			LocalStorageProvider<Memory::PlatformAllocator> _storage_provider{ _allocator };
		};
	}  // namespace

	TEST_F(LocalStorageTest, Protocol)
	{
		EXPECT_EQ(_storage_provider.Protocol(), StorageProviderProtocol::Local);
	}

	TEST_F(LocalStorageTest, Create_Stat_Remove)
	{
		StringView file_name = ORION_STRINGVIEW("./LocalStorageTestFiles/Create_Stat_Remove/1");

		IOResult<StorageStatInfo> pre_stat_result = _storage_provider.Stat(file_name);
		ASSERT_TRUE(pre_stat_result.IsError());
		EXPECT_EQ(pre_stat_result.Error(), IOError::FileDoesNotExist);

		Optional<IOError> create_result = _storage_provider.Create(file_name);
		ASSERT_FALSE(create_result.IsValue());

		IOResult<StorageStatInfo> stat_result = _storage_provider.Stat(file_name);
		ASSERT_TRUE(stat_result.IsValue());
		EXPECT_EQ(stat_result->file_name, String(file_name.Data(), file_name.Size()));
		EXPECT_EQ(stat_result->size_in_bytes, 0UL);
		EXPECT_NE(stat_result->time_created, 0UL);
		EXPECT_NE(stat_result->time_last_accessed, 0UL);
		EXPECT_NE(stat_result->time_last_modified, 0UL);

		Optional<IOError> remove_result = _storage_provider.Remove(file_name);
		ASSERT_FALSE(remove_result.IsValue());

		IOResult<StorageStatInfo> post_stat_result = _storage_provider.Stat(file_name);
		ASSERT_TRUE(post_stat_result.IsError());
		EXPECT_EQ(post_stat_result.Error(), IOError::FileDoesNotExist);
	}

	TEST_F(LocalStorageTest, Create_TryToOverrideExistingFile)
	{
		StringView file_name = ORION_STRINGVIEW("./LocalStorageTestFiles/Create_TryToOverrideExistingFile/1");

		IOResult<StorageStatInfo> pre_stat_result = _storage_provider.Stat(file_name);
		ASSERT_TRUE(pre_stat_result.IsError());
		EXPECT_EQ(pre_stat_result.Error(), IOError::FileDoesNotExist);

		Optional<IOError> create_result = _storage_provider.Create(file_name);
		ASSERT_FALSE(create_result.IsValue());

		IOResult<StorageStatInfo> stat_result = _storage_provider.Stat(file_name);
		ASSERT_TRUE(stat_result.IsValue());
		EXPECT_EQ(stat_result->file_name, String(file_name.Data(), file_name.Size()));
		EXPECT_EQ(stat_result->size_in_bytes, 0UL);
		EXPECT_NE(stat_result->time_created, 0UL);
		EXPECT_NE(stat_result->time_last_accessed, 0UL);
		EXPECT_NE(stat_result->time_last_modified, 0UL);

		Optional<IOError> override_create_result = _storage_provider.Create(file_name);
		ASSERT_TRUE(override_create_result.IsValue());
		EXPECT_EQ(*override_create_result, IOError::FileCreationFailed);

		Optional<IOError> remove_result = _storage_provider.Remove(file_name);
		ASSERT_FALSE(remove_result.IsValue());

		IOResult<StorageStatInfo> post_stat_result = _storage_provider.Stat(file_name);
		ASSERT_TRUE(post_stat_result.IsError());
		EXPECT_EQ(post_stat_result.Error(), IOError::FileDoesNotExist);
	}

	TEST_F(LocalStorageTest, Write_Read) {}

	TEST_F(LocalStorageTest, List)
	{
		static constexpr CString k_list_path = "./LocalStorageTestFiles/List";
		static constexpr Array k_file_names  = { "./LocalStorageTestFiles/List/1",
			                                     "./LocalStorageTestFiles/List/2",
			                                     "./LocalStorageTestFiles/List/3",
			                                     "./LocalStorageTestFiles/List/Sub/4",
			                                     "./LocalStorageTestFiles/List/Sub/Sub/5" };

		// Verify that the destination directory does not contain ANY files beforehand.
		Vector<StorageStatInfo> pre_creation_files = _storage_provider.List(ORION_STRINGVIEW(k_list_path), false);
		ASSERT_EQ(pre_creation_files.Size(), 0UL);
		for (USize index = 0; index < k_file_names.Size(); ++index) {
			IOResult<StorageStatInfo> stat_result = _storage_provider.Stat(ORION_STRINGVIEW(k_file_names[index]));
			ASSERT_TRUE(stat_result.IsError());
			EXPECT_EQ(stat_result.Error(), IOError::FileDoesNotExist);
		}

		// Create the directory structure.
		for (USize index = 0; index < k_file_names.Size(); ++index) {
			Optional<IOError> create_result = _storage_provider.Create(ORION_STRINGVIEW(k_file_names[index]));
			EXPECT_FALSE(create_result.IsValue()) << "Failed to create a file: " << k_file_names[index];
		}

		// List all files (non-recursively) in the base directory (only 3 should be visible).
		Vector<StorageStatInfo> post_creation_files = _storage_provider.List(ORION_STRINGVIEW(k_list_path), false);
		ASSERT_FALSE(post_creation_files.IsEmpty());
		ASSERT_EQ(post_creation_files.Size(), 3UL);

		// NOTE: We cannot assume the order of the files returned by the filesystem, so sort them manually.
		post_creation_files.Sort([](const StorageStatInfo& lhs, const StorageStatInfo& rhs) -> Bool8 {
			return lhs.file_name < rhs.file_name;
		});

		for (USize index = 0; index < post_creation_files.Size(); ++index) {
			EXPECT_EQ(post_creation_files[index].file_name, ORION_STRING(k_file_names[index]));
			EXPECT_EQ(post_creation_files[index].size_in_bytes, 0UL);
			EXPECT_NE(post_creation_files[index].time_created, 0UL);
			EXPECT_NE(post_creation_files[index].time_last_accessed, 0UL);
			EXPECT_NE(post_creation_files[index].time_last_modified, 0UL);
		}

		// Cleanup afterwards the directory.
		for (USize index = 0; index < k_file_names.Size(); ++index) {
			Optional<IOError> remove_result = _storage_provider.Remove(ORION_STRINGVIEW(k_file_names[index]));
			EXPECT_FALSE(remove_result.IsValue()) << "Failed to remove a file: " << k_file_names[index];
		}
	}

	TEST_F(LocalStorageTest, List_Recursive)
	{
		static constexpr CString k_list_path = "./LocalStorageTestFiles/List_Recursive";
		static constexpr Array k_file_names  = { "./LocalStorageTestFiles/List_Recursive/1",
			                                     "./LocalStorageTestFiles/List_Recursive/2",
			                                     "./LocalStorageTestFiles/List_Recursive/3",
			                                     "./LocalStorageTestFiles/List_Recursive/Sub/4",
			                                     "./LocalStorageTestFiles/List_Recursive/Sub/Sub/5" };

		// Verify that the destination directory does not contain ANY files beforehand.
		Vector<StorageStatInfo> pre_creation_files = _storage_provider.List(ORION_STRINGVIEW(k_list_path), true);
		ASSERT_EQ(pre_creation_files.Size(), 0UL);
		for (USize index = 0; index < k_file_names.Size(); ++index) {
			IOResult<StorageStatInfo> stat_result = _storage_provider.Stat(ORION_STRINGVIEW(k_file_names[index]));
			ASSERT_TRUE(stat_result.IsError());
			EXPECT_EQ(stat_result.Error(), IOError::FileDoesNotExist);
		}

		// Create the directory structure.
		for (USize index = 0; index < k_file_names.Size(); ++index) {
			Optional<IOError> create_result = _storage_provider.Create(ORION_STRINGVIEW(k_file_names[index]));
			EXPECT_FALSE(create_result.IsValue()) << "Failed to create a file: " << k_file_names[index];
		}

		// List all files (non-recursively) in the base directory (only 3 should be visible).
		Vector<StorageStatInfo> post_creation_files = _storage_provider.List(ORION_STRINGVIEW(k_list_path), true);
		ASSERT_FALSE(post_creation_files.IsEmpty());
		ASSERT_EQ(post_creation_files.Size(), 5UL);

		// NOTE: We cannot assume the order of the files returned by the filesystem, so sort them manually.
		post_creation_files.Sort([](const StorageStatInfo& lhs, const StorageStatInfo& rhs) -> Bool8 {
			return lhs.file_name < rhs.file_name;
		});

		for (USize index = 0; index < post_creation_files.Size(); ++index) {
			EXPECT_EQ(post_creation_files[index].file_name, ORION_STRING(k_file_names[index]));
			EXPECT_EQ(post_creation_files[index].size_in_bytes, 0UL);
			EXPECT_NE(post_creation_files[index].time_created, 0UL);
			EXPECT_NE(post_creation_files[index].time_last_accessed, 0UL);
			EXPECT_NE(post_creation_files[index].time_last_modified, 0UL);
		}

		// Cleanup afterwards the directory.
		for (USize index = 0; index < k_file_names.Size(); ++index) {
			Optional<IOError> remove_result = _storage_provider.Remove(ORION_STRINGVIEW(k_file_names[index]));
			EXPECT_FALSE(remove_result.IsValue()) << "Failed to remove a file: " << k_file_names[index];
		}
	}
}  // namespace Orion::Engine::Platform::FileSystem
