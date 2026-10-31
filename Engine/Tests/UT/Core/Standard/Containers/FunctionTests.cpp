#include "TestCommon.h"

#include "Core/Standard/Containers/Function.h"

namespace Orion::Engine::UT
{
	namespace
	{
		class FunctionTest : public ::testing::Test
		{
		};
	}  // namespace

	struct FunctionTestHelperClass
	{
		static Int32 Value() noexcept
		{
			return 5;
		}
	};

	static Int32 FunctionTestGetValue() noexcept
	{
		return 8;
	}

	TEST_F(FunctionTest, Constructor_Default)
	{
		Function<void()> fn{};
		ASSERT_FALSE(fn.IsValid());
	}

	TEST_F(FunctionTest, Constructor_Copy)
	{
		auto test_function = [](USize index) -> USize { return index; };

		Function<USize(USize)> f1{ test_function };
		ASSERT_TRUE(f1.IsValid());
		EXPECT_EQ(f1.operator()(1UL), 1UL);

		Function f2(f1);

		ASSERT_TRUE(f1.IsValid());
		ASSERT_TRUE(f2.IsValid());
		EXPECT_EQ(f1.operator()(2UL), 2UL);
		EXPECT_EQ(f2.operator()(3UL), 3UL);
	}

	TEST_F(FunctionTest, Constructor_Move)
	{
		auto test_function = [](USize index) -> USize { return index; };

		Function<USize(USize)> f1{ test_function };
		ASSERT_TRUE(f1.IsValid());
		EXPECT_EQ(f1.operator()(1UL), 1UL);

		Function f2(Move(f1));

		ASSERT_TRUE(f2.IsValid());
		EXPECT_EQ(f2.operator()(3UL), 3UL);
	}

	TEST_F(FunctionTest, Constructor_Function_Lambda)
	{
		auto test_function = [](USize index) -> USize { return index; };

		Function<USize(USize)> f1{ test_function };
		ASSERT_TRUE(f1.IsValid());
		EXPECT_EQ(f1.operator()(1UL), 1UL);
	}

	TEST_F(FunctionTest, Constructor_Function_StaticFunction)
	{
		Function<Int32()> f{ FunctionTestGetValue };
		ASSERT_TRUE(f.IsValid());
		EXPECT_EQ(f.operator()(), 8);
	}

	TEST_F(FunctionTest, Constructor_Function_MemberFunction)
	{
		Function<Int32()> f{ FunctionTestHelperClass::Value };
		ASSERT_TRUE(f.IsValid());
		EXPECT_EQ(f.operator()(), 5);
	}

	TEST_F(FunctionTest, Operator_Copy)
	{
		Function<USize(USize)> f1{ [](USize index) -> USize { return index; } };
		ASSERT_TRUE(f1.IsValid());
		EXPECT_EQ(f1.operator()(1UL), 1UL);

		Function<USize(USize)> f2{};
		ASSERT_FALSE(f2.IsValid());

		f2 = f1;
		ASSERT_TRUE(f1.IsValid());
		EXPECT_EQ(f1.operator()(8UL), 8UL);
		ASSERT_TRUE(f2.IsValid());
		EXPECT_EQ(f2.operator()(8UL), 8UL);
	}

	TEST_F(FunctionTest, Operator_Move)
	{
		Function<USize(USize)> f1{ [](USize index) -> USize { return index; } };
		ASSERT_TRUE(f1.IsValid());
		EXPECT_EQ(f1.operator()(1UL), 1UL);

		Function<USize(USize)> f2{};
		ASSERT_FALSE(f2.IsValid());

		f2 = Move(f1);
		ASSERT_TRUE(f2.IsValid());
		EXPECT_EQ(f2.operator()(8UL), 8UL);
	}

}  // namespace Orion::Engine::UT
