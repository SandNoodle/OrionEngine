#include "TestCommon.h"

#include "Core/Standard/Threads/Atomic.h"

namespace Orion::Engine::Thread::UT
{
	// NOTE: Ideally, Atomic<T> should be tested in the context of multiple threads, i.e. one thread which modifies the
	// atomic and other one who checks the value. However, under the hood this class uses bunch of builtins provided by
	// compiler for us - so in that case I don't think there's much sense in testing anything other than the interface.
	namespace
	{
		template <typename>
		class AtomicTest : public ::testing::Test
		{
		};
	}  // namespace
	TYPED_TEST_SUITE_P(AtomicTest);

	TYPED_TEST_P(AtomicTest, Constructor_Default)
	{
		Atomic<TypeParam> a{};
		TypeParam current_value  = a.Load();
		TypeParam expected_value = {};
		ASSERT_EQ(expected_value, current_value);
	}

	TYPED_TEST_P(AtomicTest, Constructor_Value)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		TypeParam current_value  = a.Load();
		TypeParam expected_value = TypeParam(3);
		ASSERT_EQ(expected_value, current_value);
	}

	TYPED_TEST_P(AtomicTest, Operator_Assign_Value)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.Load(), TypeParam(3));

		a = TypeParam(4);
		EXPECT_EQ(a.Load(), TypeParam(4));
	}

	TYPED_TEST_P(AtomicTest, Operator_Get_Value)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.operator TypeParam(), TypeParam(3));
	}

	TYPED_TEST_P(AtomicTest, Operator_Assign_Add)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.Load(), TypeParam(3));

		a += TypeParam(1);
		EXPECT_EQ(a.Load(), TypeParam(4));
	}

	TYPED_TEST_P(AtomicTest, Operator_Assign_Sub)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.Load(), TypeParam(3));

		a -= TypeParam(1);
		EXPECT_EQ(a.Load(), TypeParam(2));
	}

	TYPED_TEST_P(AtomicTest, Operator_Pre_Increment)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			++a;
			EXPECT_EQ(a.Load(), TypeParam(4));
		}
	}

	TYPED_TEST_P(AtomicTest, Operator_Post_Increment)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			a++;
			EXPECT_EQ(a.Load(), TypeParam(4));
		}
	}

	TYPED_TEST_P(AtomicTest, Operator_Pre_Decrement)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			--a;
			EXPECT_EQ(a.Load(), TypeParam(2));
		}
	}

	TYPED_TEST_P(AtomicTest, Operator_Pre_Post_Decrement)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			a--;
			EXPECT_EQ(a.Load(), TypeParam(2));
		}
	}

	TYPED_TEST_P(AtomicTest, Operator_Bitwise_And)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			a &= TypeParam(2);
			EXPECT_EQ(a.Load(), TypeParam(2));
		}
	}

	TYPED_TEST_P(AtomicTest, Operator_Bitwise_Or)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			a |= TypeParam(4);
			EXPECT_EQ(a.Load(), TypeParam(7));
		}
	}

	TYPED_TEST_P(AtomicTest, Operator_Bitwise_Xor)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(3) };
			EXPECT_EQ(a.Load(), TypeParam(3));

			a ^= TypeParam(8);
			EXPECT_EQ(a.Load(), TypeParam(11));
		}
	}

	TYPED_TEST_P(AtomicTest, IsLockFree)
	{
		Atomic<TypeParam> a{};
		EXPECT_TRUE(a.IsLockFree());
	}

	TYPED_TEST_P(AtomicTest, Store)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.Load(), TypeParam(3));

		a.Store(TypeParam(5));
		EXPECT_EQ(a.Load(), TypeParam(5));
	}

	TYPED_TEST_P(AtomicTest, Load)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.Load(), TypeParam(3));
	}

	TYPED_TEST_P(AtomicTest, Exchange)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		EXPECT_EQ(a.Load(), TypeParam(3));

		TypeParam previous_value = a.Exchange(TypeParam(5));
		EXPECT_EQ(previous_value, TypeParam(3));
		EXPECT_EQ(a.Load(), TypeParam(5));
	}

	TYPED_TEST_P(AtomicTest, CompareExchangeWeak)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		TypeParam current_value = a.Load();
		EXPECT_EQ(current_value, TypeParam(3));
		EXPECT_TRUE(a.CompareExchangeWeak(
			current_value, TypeParam(5), MemoryOrder::SequentiallyConsistent, MemoryOrder::SequentiallyConsistent));
		EXPECT_FALSE(a.CompareExchangeWeak(
			current_value, TypeParam(8), MemoryOrder::SequentiallyConsistent, MemoryOrder::SequentiallyConsistent));
	}

	TYPED_TEST_P(AtomicTest, CompareExchangeStrong)
	{
		Atomic<TypeParam> a{ TypeParam(3) };
		TypeParam current_value = a.Load();
		EXPECT_EQ(current_value, TypeParam(3));
		EXPECT_TRUE(a.CompareExchangeWeak(
			current_value, TypeParam(5), MemoryOrder::SequentiallyConsistent, MemoryOrder::SequentiallyConsistent));
		EXPECT_FALSE(a.CompareExchangeWeak(
			current_value, TypeParam(8), MemoryOrder::SequentiallyConsistent, MemoryOrder::SequentiallyConsistent));
	}

	TYPED_TEST_P(AtomicTest, FetchAdd)
	{
		Atomic<TypeParam> a{ TypeParam(2) };
		EXPECT_EQ(a.Load(), TypeParam(2));

		TypeParam previous_value = a.FetchAdd(TypeParam(1));
		EXPECT_EQ(previous_value, TypeParam(2));
		EXPECT_EQ(a.Load(), TypeParam(3));
	}

	TYPED_TEST_P(AtomicTest, FetchSub)
	{
		Atomic<TypeParam> a{ TypeParam(2) };
		EXPECT_EQ(a.Load(), TypeParam(2));

		TypeParam previous_value = a.FetchSub(TypeParam(1));
		EXPECT_EQ(previous_value, TypeParam(2));
		EXPECT_EQ(a.Load(), TypeParam(1));
	}

	TYPED_TEST_P(AtomicTest, FetchMax)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(2) };
			EXPECT_EQ(a.Load(), TypeParam(2));

			TypeParam previous_value = a.FetchMax(TypeParam(5));
			EXPECT_EQ(previous_value, TypeParam(2));
			EXPECT_EQ(a.Load(), TypeParam(5));
		}
	}

	TYPED_TEST_P(AtomicTest, FetchMin)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(2) };
			EXPECT_EQ(a.Load(), TypeParam(2));

			TypeParam previous_value = a.FetchMin(TypeParam(0));
			EXPECT_EQ(previous_value, TypeParam(2));
			EXPECT_EQ(a.Load(), TypeParam(0));
		}
	}

	TYPED_TEST_P(AtomicTest, FetchAnd)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(2) };
			EXPECT_EQ(a.Load(), TypeParam(2));

			TypeParam previous_value = a.FetchAnd(TypeParam(3));
			EXPECT_EQ(previous_value, TypeParam(2));
			EXPECT_EQ(a.Load(), TypeParam(2));
		}
	}

	TYPED_TEST_P(AtomicTest, FetchOr)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(2) };
			EXPECT_EQ(a.Load(), TypeParam(2));

			TypeParam previous_value = a.FetchOr(TypeParam(9));
			EXPECT_EQ(previous_value, TypeParam(2));
			EXPECT_EQ(a.Load(), TypeParam(11));
		}
	}

	TYPED_TEST_P(AtomicTest, FetchXor)
	{
		if constexpr (IntegralKind<TypeParam>) {
			Atomic<TypeParam> a{ TypeParam(2) };
			EXPECT_EQ(a.Load(), TypeParam(2));

			TypeParam previous_value = a.FetchXor(TypeParam(7));
			EXPECT_EQ(previous_value, TypeParam(2));
			EXPECT_EQ(a.Load(), TypeParam(5));
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(AtomicTest,
	                            Constructor_Default,
	                            Constructor_Value,
	                            Operator_Assign_Value,
	                            Operator_Get_Value,
	                            Operator_Assign_Add,
	                            Operator_Assign_Sub,
	                            Operator_Pre_Increment,
	                            Operator_Post_Increment,
	                            Operator_Pre_Decrement,
	                            Operator_Pre_Post_Decrement,
	                            Operator_Bitwise_And,
	                            Operator_Bitwise_Or,
	                            Operator_Bitwise_Xor,
	                            IsLockFree,
	                            Store,
	                            Load,
	                            Exchange,
	                            CompareExchangeWeak,
	                            CompareExchangeStrong,
	                            FetchAdd,
	                            FetchSub,
	                            FetchMax,
	                            FetchMin,
	                            FetchAnd,
	                            FetchOr,
	                            FetchXor);

	INSTANTIATE_TYPED_TEST_SUITE_P(AtomicPrimitiveTypesTest, AtomicTest, Engine::UT::PrimitiveTypes);
}  // namespace Orion::Engine::Thread::UT
