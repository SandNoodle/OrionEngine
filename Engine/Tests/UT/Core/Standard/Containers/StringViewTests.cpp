#include "TestCommon.h"

#include "Core/Standard/Containers/StringView.h"

namespace Orion::Engine::UT
{
	using StringViewTestTypes = ::testing::Types<StringView, StringViewUTF8, StringViewUTF16, StringViewUTF32>;

	static constexpr CString k_cstring_literal    = "Some C-styled string literal.";
	static constexpr USize k_cstring_literal_size = StringLength<Detail::StringEncoding::ANSI>(k_cstring_literal);

	namespace
	{
		template <typename T>
		class StringViewTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE(StringViewTest, StringViewTestTypes);

	TYPED_TEST(StringViewTest, Constructor_Default)
	{
		TypeParam sv{};
		EXPECT_TRUE(sv.IsEmpty());
		EXPECT_EQ(sv.Size(), 0);
		EXPECT_EQ(sv.ByteSize(), 0);
	}

	TYPED_TEST(StringViewTest, Constructor_FromStringLiteral)
	{
		if constexpr (!IsSame<TypeParam, StringView>) {
			GTEST_SKIP() << "C-styled string literals only make sense for ANSI StringView.";
		}

		TypeParam sv{ k_cstring_literal };
		EXPECT_FALSE(sv.IsEmpty());
		EXPECT_EQ(sv.Size(), k_cstring_literal_size);
		EXPECT_EQ(sv.ByteSize(), k_cstring_literal_size * sizeof(typename TypeParam::CharType));

		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			EXPECT_EQ(sv[index], k_cstring_literal[index]);
		}
	}

	TYPED_TEST(StringViewTest, Constructor_WithPointerAndSize)
	{
		Array<typename TypeParam::CharType, k_cstring_literal_size> character_buffer = {};
		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			character_buffer[index] = static_cast<TypeParam::ValueType>(k_cstring_literal[index]);
		}

		TypeParam sv(character_buffer.Data(), k_cstring_literal_size);
		EXPECT_FALSE(sv.IsEmpty());
		EXPECT_EQ(sv.Size(), k_cstring_literal_size);
		EXPECT_EQ(sv.ByteSize(), k_cstring_literal_size * sizeof(typename TypeParam::CharType));
		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			EXPECT_EQ(sv[index], character_buffer[index]);
		}
	}

	TYPED_TEST(StringViewTest, Constructor_FromRange)
	{
		Array<typename TypeParam::CharType, k_cstring_literal_size> character_buffer = {};
		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			character_buffer[index] = static_cast<TypeParam::ValueType>(k_cstring_literal[index]);
		}

		TypeParam sv(character_buffer.begin(), character_buffer.end());
		EXPECT_FALSE(sv.IsEmpty());
		EXPECT_EQ(sv.Size(), k_cstring_literal_size);
		EXPECT_EQ(sv.ByteSize(), k_cstring_literal_size * sizeof(typename TypeParam::CharType));
		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			EXPECT_EQ(sv[index], character_buffer[index]);
		}
	}

	TYPED_TEST(StringViewTest, SubView)
	{
		Array<typename TypeParam::CharType, k_cstring_literal_size> character_buffer = {};
		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			character_buffer[index] = static_cast<TypeParam::ValueType>(k_cstring_literal[index]);
		}

		TypeParam sv{ character_buffer.begin(), character_buffer.end() };
		EXPECT_FALSE(sv.IsEmpty());
		EXPECT_EQ(sv.Size(), k_cstring_literal_size);
		EXPECT_EQ(sv.ByteSize(), k_cstring_literal_size * sizeof(typename TypeParam::CharType));

		TypeParam ssv = sv.SubView(0, k_cstring_literal_size - 4);
		EXPECT_FALSE(ssv.IsEmpty());
		EXPECT_EQ(ssv.Size(), k_cstring_literal_size - 4);
		EXPECT_EQ(ssv.ByteSize(), (k_cstring_literal_size - 4) * sizeof(typename TypeParam::CharType));
		for (USize index = 0; index < k_cstring_literal_size - 4; ++index) {
			EXPECT_EQ(ssv[index], sv[index]);
		}
	}

	TYPED_TEST(StringViewTest, Find)
	{
		Array<typename TypeParam::CharType, k_cstring_literal_size> character_buffer = {};
		for (USize index = 0; index < k_cstring_literal_size; ++index) {
			character_buffer[index] = static_cast<TypeParam::ValueType>(k_cstring_literal[index]);
		}

		TypeParam sv{ character_buffer.begin(), character_buffer.end() };
		EXPECT_FALSE(sv.IsEmpty());
		EXPECT_EQ(sv.Size(), k_cstring_literal_size);
		EXPECT_EQ(sv.ByteSize(), k_cstring_literal_size * sizeof(typename TypeParam::CharType));

#define ORION_STRINGVIEW_TEST_SEARCH_PATTERN(character)                               \
	static constexpr Array<typename TypeParam::CharType, 1> k_symbol = { character }; \
	TypeParam search_pattern{ k_symbol.Data(), k_symbol.Size() };

		{
			// NOTE: Should be present in the input string.
			ORION_STRINGVIEW_TEST_SEARCH_PATTERN('-')
			typename TypeParam::SizeType index = sv.Find(search_pattern, 0UL);
			EXPECT_EQ(index, 6UL);
		}

		{
			// NOTE: Should NOT be present in the input string.
			ORION_STRINGVIEW_TEST_SEARCH_PATTERN('@')
			typename TypeParam::SizeType index = sv.Find(search_pattern, 0UL);
			EXPECT_EQ(index, decltype(sv)::k_invalid_index);
		}

		{
			// NOTE: Should be present in the input string, but outside the starting point.
			ORION_STRINGVIEW_TEST_SEARCH_PATTERN('-')
			typename TypeParam::SizeType index = sv.Find(search_pattern, 15UL);
			EXPECT_EQ(index, decltype(sv)::k_invalid_index);
		}
	}
}  // namespace Orion::Engine::UT
