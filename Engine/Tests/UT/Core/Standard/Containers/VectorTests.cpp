#include "TestCommon.h"

#include "Core/Memory/Allocators/DummyAllocator.h"
#include "Core/Standard/Algorithms/Sort.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/Vector.h"

#include <ostream>

namespace Orion::Engine::UT
{
	class ComplexType
	{
		public:
		Float32 x;
		Float32 y;

		public:
		constexpr ComplexType() : x(0), y(0) {}
		constexpr ComplexType(Float32 x, Float32 y) : x(x), y(y) {}
		virtual ~ComplexType() {}

		ComplexType& operator*=(const ComplexType& other)
		{
			x *= other.x;
			y *= other.y;
			return *this;
		}
	};

	[[nodiscard]] constexpr bool operator==(const ComplexType& lhs, const ComplexType& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}

	[[nodiscard]] constexpr bool operator>(const ComplexType& lhs, const ComplexType& rhs)
	{
		return lhs.x > rhs.x && lhs.y > rhs.y;
	}

	[[nodiscard]] constexpr ComplexType operator*(const ComplexType& lhs, const ComplexType& rhs)
	{
		return ComplexType(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	std::ostream& operator<<(std::ostream& os, const ComplexType& t)
	{
		os << '[' << t.x << ", " << t.y << ']';
		return os;
	}

	template <typename>
	class VectorTest : public ::testing::Test
	{
	};
	TYPED_TEST_SUITE_P(VectorTest);

	template <typename T>
	static constexpr T Value(USize index)
	{
		return static_cast<T>(index);
	}

	template <>
	constexpr ComplexType Value<ComplexType>(USize index)
	{
		return ComplexType(static_cast<Float32>(index), static_cast<Float32>(index));
	}

	template <typename T>
	static constexpr Vector<T> GetVector(USize size, const T& multiplier = Value<T>(1))
	{
		Vector<T> v;
		v.Reserve(size);
		for (USize index = 0; index < size; ++index) {
			v.Add(Value<T>(index) * multiplier);
		}
		return v;
	}

	template <typename T, USize Size>
	static constexpr void Verify(const Array<T, Size>& expected, const Vector<T>& result)
	{
		if constexpr (Size == 0) {
			EXPECT_TRUE(result.IsEmpty());
		} else {
			EXPECT_FALSE(result.IsEmpty());
		}

		EXPECT_EQ(expected.Size(), result.Size())
			<< "Expected size: " << expected.Size() << ", but got: " << result.Size();
		EXPECT_EQ(expected.ByteSize(), result.ByteSize())
			<< "Expected byte size: " << expected.ByteSize() << ", but got: " << result.ByteSize();

		for (USize index = 0; index < Size; ++index) {
			EXPECT_EQ(expected[index], result[index])
				<< "(operator[]) Expected: " << expected[index] << " but got: " << result[index];
			EXPECT_EQ(*(expected.Data() + index), *(result.Data() + index))
				<< "(Data()) Expected: " << expected[index] << " but got: " << result[index];
		}

		if constexpr (Size > 0) {
			auto* expected_it = expected.begin();
			auto* result_it   = result.begin();
			while (expected_it != expected.end()) {
				EXPECT_EQ(*expected_it, *result_it)
					<< "(Iterator) Expected: " << *expected_it << " but got: " << *result_it;
				++expected_it;
				++result_it;
			}

			EXPECT_EQ(expected.Front(), result.Front())  //
				<< "Expected: " << expected.Front() << " but got: " << result.Front();
			EXPECT_EQ(expected.Back(), result.Back())  //
				<< "Expected: " << expected.Back() << " but got: " << result.Back();
		}
	}

	TYPED_TEST_P(VectorTest, Constructor_InitialCapacity)
	{
		Vector v = Vector<TypeParam, DummyAllocator>(8, DummyAllocator());
		EXPECT_TRUE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 0);
		EXPECT_EQ(v.ByteSize(), 0);
		EXPECT_EQ(v.Capacity(), 8);
	}

	TYPED_TEST_P(VectorTest, Constructor_InitializerList_Empty)
	{
		Vector<TypeParam, DummyAllocator> v({});
		EXPECT_TRUE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 0);
		EXPECT_EQ(v.ByteSize(), 0);
		EXPECT_EQ(v.Capacity(), decltype(v)::k_initial_capacity);
	}

	TYPED_TEST_P(VectorTest, Constructor_InitializerList)
	{
		Vector<TypeParam> v({ Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) });

		EXPECT_FALSE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 3);
		EXPECT_EQ(v.ByteSize(), v.Size() * sizeof(TypeParam));
		EXPECT_EQ(v.Capacity(), decltype(v)::k_initial_capacity);
	}

	TYPED_TEST_P(VectorTest, Constructor_Copy)
	{
		static const Array k_expected_values = {
			Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2), Value<TypeParam>(3), Value<TypeParam>(4),
		};

		Vector v1 = GetVector<TypeParam>(5);
		Verify(k_expected_values, v1);

		Vector v2 = v1;
		Verify(k_expected_values, v2);
	}

	TYPED_TEST_P(VectorTest, Constructor_Move)
	{
		static const Array k_expected_values = {
			Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2), Value<TypeParam>(3), Value<TypeParam>(4),
		};

		Vector v1 = GetVector<TypeParam>(5);
		Verify(k_expected_values, v1);

		Vector v2 = Move(v1);
		Verify(k_expected_values, v2);
	}

	TYPED_TEST_P(VectorTest, Operator_Copy)
	{
		static const TypeParam k_multiplier = Value<TypeParam>(3);
		static const Array k_expected_v1    = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static const Array k_expected_v2
			= { Value<TypeParam>(0), Value<TypeParam>(3), Value<TypeParam>(6), Value<TypeParam>(9) };

		Vector v1 = GetVector<TypeParam>(3);
		Vector v2 = GetVector<TypeParam>(4, k_multiplier);

		Verify(k_expected_v1, v1);
		Verify(k_expected_v2, v2);

		v2 = v1;

		Verify(k_expected_v1, v1);
		Verify(k_expected_v1, v2);
	}

	TYPED_TEST_P(VectorTest, Operator_Move)
	{
		static const TypeParam k_multiplier = Value<TypeParam>(3);
		static const Array k_expected_v1    = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static const Array k_expected_v2
			= { Value<TypeParam>(0), Value<TypeParam>(3), Value<TypeParam>(6), Value<TypeParam>(9) };

		Vector v1 = GetVector<TypeParam>(3);
		Vector v2 = GetVector<TypeParam>(4, k_multiplier);

		Verify(k_expected_v1, v1);
		Verify(k_expected_v2, v2);

		v2 = Move(v1);
		Verify(k_expected_v1, v2);
	}

	TYPED_TEST_P(VectorTest, Capacity_EmptyVector)
	{
		Vector<TypeParam> v{};
		EXPECT_TRUE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 0);
		EXPECT_EQ(v.ByteSize(), 0);
		EXPECT_EQ(v.Capacity(), decltype(v)::k_initial_capacity);
	}

	TYPED_TEST_P(VectorTest, Capacity_NonEmptyVector)
	{
		Vector v = GetVector<TypeParam>(5);

		EXPECT_FALSE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 5);
		EXPECT_EQ(v.ByteSize(), sizeof(TypeParam) * v.Size());
		EXPECT_EQ(v.Capacity(), decltype(v)::k_initial_capacity);
	}

	TYPED_TEST_P(VectorTest, Capacity_Reserve)
	{
		Vector v = GetVector<TypeParam>(5);

		EXPECT_FALSE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 5);
		EXPECT_EQ(v.ByteSize(), sizeof(TypeParam) * v.Size());
		EXPECT_EQ(v.Capacity(), decltype(v)::k_initial_capacity);

		v.Reserve(12);
		EXPECT_FALSE(v.IsEmpty());
		EXPECT_EQ(v.Size(), 5);
		EXPECT_GE(v.Capacity(), 12);
	}

	TYPED_TEST_P(VectorTest, ElementAccess_Read)
	{
		static const Array k_expected_values = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };

		Vector v = GetVector<TypeParam>(3);
		Verify(k_expected_values, v);
	}

	TYPED_TEST_P(VectorTest, ElementAccess_Write)
	{
		static const TypeParam k_multiplier  = Value<TypeParam>(2);
		static const Array k_expected_before = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static const Array k_expected_after  = { Value<TypeParam>(0), Value<TypeParam>(2), Value<TypeParam>(4) };

		Vector v = GetVector<TypeParam>(3);
		Verify(k_expected_before, v);
		for (USize index = 0; index < v.Size(); ++index) {
			v[index] *= k_multiplier;
		}
		Verify(k_expected_after, v);
	}

	TYPED_TEST_P(VectorTest, Elements_Modification)
	{
		static const Array<TypeParam, 0> k_expected_empty = {};
		static const Array k_expected_add
			= { Value<TypeParam>(5), Value<TypeParam>(4), Value<TypeParam>(0), Value<TypeParam>(0) };
		static const Array k_expected_remove = { Value<TypeParam>(5), Value<TypeParam>(4) };

		Vector<TypeParam> v;
		Verify(k_expected_empty, v);

		v.Add(Value<TypeParam>(5));
		v.AddConstruct(Value<TypeParam>(4));
		v.AddZeroed(2);
		Verify(k_expected_add, v);

		v.Remove(2);
		v.RemoveBack();
		v.Sort(Algorithm::Compare::Greater<TypeParam>);  // NOTE: When removing elements there is no guarantee that the
		                                                 // order will be preserved.
		Verify(k_expected_remove, v);

		v.Clear();
		Verify(k_expected_empty, v);
	}

	TYPED_TEST_P(VectorTest, Sort)
	{
		static const Array k_expected_before = { Value<TypeParam>(0), Value<TypeParam>(1), Value<TypeParam>(2) };
		static const Array k_expected_after  = { Value<TypeParam>(2), Value<TypeParam>(1), Value<TypeParam>(0) };

		Vector v = GetVector<TypeParam>(3);
		Verify(k_expected_before, v);
		v.Sort(Algorithm::Compare::Greater<TypeParam>);
		Verify(k_expected_after, v);
	}

	REGISTER_TYPED_TEST_SUITE_P(VectorTest,
	                            Constructor_InitialCapacity,
	                            Constructor_InitializerList_Empty,
	                            Constructor_InitializerList,
	                            Constructor_Copy,
	                            Constructor_Move,
	                            Operator_Copy,
	                            Operator_Move,
	                            Capacity_EmptyVector,
	                            Capacity_NonEmptyVector,
	                            Capacity_Reserve,
	                            ElementAccess_Read,
	                            ElementAccess_Write,
	                            Elements_Modification,
	                            Sort);

	INSTANTIATE_TYPED_TEST_SUITE_P(VectorPrimitiveTypesTest, VectorTest, PrimitiveTypes);
	INSTANTIATE_TYPED_TEST_SUITE_P(VectorComplexTypesTest, VectorTest, ::testing::Types<ComplexType>);
}  // namespace Orion::Engine::UT
