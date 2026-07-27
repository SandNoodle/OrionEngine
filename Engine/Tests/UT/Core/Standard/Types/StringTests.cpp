#include "TestCommon.h"

#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/TypeTraits.h"
#include "Core/Standard/Types/String.h"
#include "Core/Standard/Utility/StringUtils.h"

namespace Orion::Engine::UT
{
	using StringTestTypes = ::testing::Types</*String, StringUTF8, */ StringUTF16, StringUTF32>;

	namespace
	{
		template <typename T>
		class StringTest : public ::testing::Test
		{
			protected:
			static constexpr CString k_cstring_literal = "Some C-styled string literal.";
			static constexpr USize k_cstring_literal_size
				= StringLength<Detail::StringEncoding::ANSI>(k_cstring_literal);
		};
	}  // namespace
	TYPED_TEST_SUITE(StringTest, StringTestTypes);

	TYPED_TEST(StringTest, Constructor_Default)
	{
		TypeParam s{};
		EXPECT_TRUE(s.IsEmpty());
		EXPECT_EQ(s.Size(), 0);
		EXPECT_EQ(s.ByteSize(), 0);
	}

	TYPED_TEST(StringTest, Constructor_FromStringLiteral)
	{
		TypeParam s{ this->k_cstring_literal };
		EXPECT_FALSE(s.IsEmpty());
		EXPECT_EQ(s.Size(), this->k_cstring_literal_size);
		EXPECT_EQ(s.ByteSize(), this->k_cstring_literal_size * sizeof(typename TypeParam::CharType));

		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s[index], this->k_cstring_literal[index]);
		}
	}

	TYPED_TEST(StringTest, Constructor_WithPointerAndSize)
	{
		typename TypeParam::ValueType character_buffer[128] = {};
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			character_buffer[index] = static_cast<TypeParam::ValueType>(this->k_cstring_literal[index]);
		}

		TypeParam s(character_buffer, this->k_cstring_literal_size);
		EXPECT_FALSE(s.IsEmpty());
		EXPECT_EQ(s.Size(), this->k_cstring_literal_size);
		EXPECT_EQ(s.ByteSize(), this->k_cstring_literal_size * sizeof(typename TypeParam::CharType));

		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s[index], character_buffer[index]);
		}
	}

	TYPED_TEST(StringTest, Constructor_Copy)
	{
		TypeParam s1(this->k_cstring_literal);
		EXPECT_FALSE(s1.IsEmpty());
		ASSERT_EQ(s1.Size(), this->k_cstring_literal_size);
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s1[index], this->k_cstring_literal[index]);
		}

		TypeParam s2(s1);
		EXPECT_FALSE(s2.IsEmpty());
		ASSERT_EQ(s1.Size(), s2.Size());
		for (USize index = 0; index < s2.Size(); ++index) {
			EXPECT_EQ(s1[index], s2[index]);
		}
	}

	TYPED_TEST(StringTest, Constructor_Move)
	{
		TypeParam s1(this->k_cstring_literal);
		EXPECT_FALSE(s1.IsEmpty());
		ASSERT_EQ(s1.Size(), this->k_cstring_literal_size);
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s1[index], this->k_cstring_literal[index]);
		}

		TypeParam s2(Move(s1));
		EXPECT_FALSE(s2.IsEmpty());
		ASSERT_EQ(s2.Size(), this->k_cstring_literal_size);
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s2[index], this->k_cstring_literal[index]);
		}
	}

	TYPED_TEST(StringTest, Operator_Copy)
	{
		TypeParam s1(this->k_cstring_literal);
		EXPECT_FALSE(s1.IsEmpty());
		ASSERT_EQ(s1.Size(), this->k_cstring_literal_size);
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s1[index], this->k_cstring_literal[index]);
		}

		TypeParam s2;
		EXPECT_TRUE(s2.IsEmpty());
		EXPECT_EQ(s2.Size(), 0);
		EXPECT_EQ(s2.ByteSize(), 0);

		s2 = s1;
		EXPECT_FALSE(s2.IsEmpty());
		ASSERT_EQ(s1.Size(), s2.Size());
		for (USize index = 0; index < s2.Size(); ++index) {
			EXPECT_EQ(s1[index], s2[index]);
		}
	}

	TYPED_TEST(StringTest, Operator_Move)
	{
		TypeParam s1(this->k_cstring_literal);
		EXPECT_FALSE(s1.IsEmpty());
		ASSERT_EQ(s1.Size(), this->k_cstring_literal_size);
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s1[index], this->k_cstring_literal[index]);
		}

		TypeParam s2;
		EXPECT_TRUE(s2.IsEmpty());
		EXPECT_EQ(s2.Size(), 0);
		EXPECT_EQ(s2.ByteSize(), 0);

		s2 = Move(s1);
		EXPECT_FALSE(s2.IsEmpty());
		ASSERT_EQ(s2.Size(), this->k_cstring_literal_size);
		for (USize index = 0; index < this->k_cstring_literal_size; ++index) {
			EXPECT_EQ(s2[index], this->k_cstring_literal[index]);
		}
	}

	TYPED_TEST(StringTest, Modification)
	{
		TypeParam s{};
		EXPECT_TRUE(s.IsEmpty());
		EXPECT_EQ(s.Size(), 0);
		EXPECT_EQ(s.ByteSize(), 0);

		static constexpr USize k_buffer_size                = 64UL;
		typename TypeParam::ValueType buffer[k_buffer_size] = {};
		for (USize index = 0; index < k_buffer_size; ++index) {
			buffer[index] = static_cast<TypeParam::ValueType>(index);
		}

		USize expected_length    = 0UL;
		USize expected_size      = 0UL;
		USize expected_byte_size = 0UL;
		{
			expected_length += 1;
			expected_size += 1;
			expected_byte_size += sizeof(typename TypeParam::CharType);

			s.Append('a');
			EXPECT_FALSE(s.IsEmpty());
			EXPECT_EQ(s.Size(), expected_size);
			EXPECT_EQ(s.ByteSize(), expected_byte_size);
			EXPECT_EQ(s.Length(), expected_length);
			EXPECT_EQ(s.GetChar(0), 'a');
			EXPECT_EQ(s[0], 'a');
		}

		{
			expected_length += this->k_cstring_literal_size;
			expected_size += this->k_cstring_literal_size;
			expected_byte_size += this->k_cstring_literal_size * sizeof(typename TypeParam::CharType);

			s.Append(this->k_cstring_literal);
			EXPECT_FALSE(s.IsEmpty());
			EXPECT_EQ(s.Size(), expected_size);
			EXPECT_EQ(s.ByteSize(), expected_byte_size);
			EXPECT_EQ(s.Length(), expected_length);

			USize previous_index = expected_size - this->k_cstring_literal_size;
			for (USize index = previous_index; index < expected_size; ++index) {
				EXPECT_EQ(s.GetChar(index),
				          static_cast<TypeParam::WideCharType>(this->k_cstring_literal[index - previous_index]));
				EXPECT_EQ(s[index], static_cast<TypeParam::CharType>(this->k_cstring_literal[index - previous_index]));
			}
		}

		{
			// static constexpr Detail::StringEncoding k_encoding = TypeParam::TraitType::k_encoding;
			// Detail::StringBase<k_encoding>;
			// s.Append();
		}

		{
			// s.AppendRange();
		}

		// s.AppendRange(buffer, buffer + k_buffer_size);
	}
}  // namespace Orion::Engine::UT
