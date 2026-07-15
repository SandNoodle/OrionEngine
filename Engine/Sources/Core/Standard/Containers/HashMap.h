#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Memory/Allocators/PlatformAllocator.h"
#include "Core/Standard/Algorithms/Hash.h"
#include "Core/Standard/Types/Pair.h"

#include <initializer_list>

namespace Orion::Engine
{
	namespace Detail
	{
		/// @brief TODO
		enum class HashMapElementState : UInt8
		{
			Free,
			Allocated,
		};

		/// @brief TODO
		template <typename T1, typename T2>
		struct HashMapElement
		{
			T1 key;
			T2 value;
			HashMapElementState state;
		};
	}  // namespace Detail

	template <typename T>
	struct Predicate;

	/**
	 * @brief TODO
	 *
	 * @warning
	 *
	 * @tparam Key
	 * @tparam Value
	 * @tparam Hash
	 * @tparam Predicate
	 * @tparam Allocator
	 */
	template <typename Key,
	          typename Value,
	          typename Hash      = Hash<Key>,
	          typename Predicate = Predicate<Key>,
	          typename Allocator = PlatformAllocator>
	class HashMap
	{
		public:
		using KeyType            = Key;
		using ValueType          = Value;
		using SizeType           = USize;
		using AllocatorType      = Allocator;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;

		using IteratorType      = PointerType;       // TODO Iterators
		using ConstIteratorType = ConstPointerType;  // TODO Iterators

		static constexpr SizeType k_initial_bucket_count   = 8UL;
		static constexpr SizeType k_load_factor_percentage = 80;

		private:
		using ElementType = Detail::HashMapElement<KeyType, ValueType>;

		private:
		AllocatorType _allocator;
		ElementType* _data;
		SizeType _capacity;
		SizeType _size;

		public:
		constexpr explicit HashMap(SizeType initial_buckets       = k_initial_bucket_count,
		                           const AllocatorType& allocator = AllocatorType());
		constexpr HashMap(std::initializer_list<ValueType>) noexcept;
		constexpr HashMap(const HashMap&) noexcept;
		constexpr HashMap(HashMap&&) noexcept;

		[[nodiscard]] constexpr HashMap& operator=(const HashMap&) noexcept;
		[[nodiscard]] constexpr HashMap& operator=(HashMap&&) noexcept;
		[[nodiscard]] constexpr HashMap& operator=(std::initializer_list<ValueType>) noexcept;

		/** Adds new element to the hashmap. If the key already exists, it will be overridden.  */
		///@{
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Add(const KeyType& key,
		                                                             const ValueType& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Add(const KeyType& key, ValueType&& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Add(KeyType&& key, const ValueType& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Add(KeyType&& key, ValueType&& value) noexcept;
		///@}

		/** Removes specified element from the container. Calls destructor. */
		///@{
		constexpr void Remove(const KeyType& key) noexcept;
		constexpr void Remove(KeyType&& key) noexcept;
		///@}

		/** Finds the value associated with a specific key - returns nullptr if the value was not found.. */
		///@{
		[[nodiscard]] ORION_FORCE_INLINE constexpr PointerType Find(const KeyType& key) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstPointerType Find(const KeyType& key) const noexcept;
		///@}

		[[nodiscard]] constexpr Bool8 Contains(const KeyType& key) const noexcept;

		/// @brief Clears the container from all the elements. Calls destructor on each of them.
		constexpr void Clear() noexcept;

		/// @brief Verifies if the container is empty, i.e. can store only 0 elements.
		[[nodiscard]] ORION_FORCE_INLINE constexpr Bool8 IsEmpty() const noexcept;

		/// @brief Returns the amount of elements that can be stored in this container.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Size() const noexcept;

		/// @brief Returns the amount of bytes that this container stores.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType ByteSize() const noexcept;

		/// @brief Returns the maximum amount of elements that can be currently stored in this container.
		[[nodiscard]] ORION_FORCE_INLINE constexpr SizeType Capacity() const noexcept;

		public:
		// NOLINTBEGIN(readability-identifier-naming)
		/** Required overload for the C++'s for range loops. */
		///@{
		[[nodiscard]] ORION_FORCE_INLINE constexpr IteratorType begin() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstIteratorType begin() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr IteratorType end() noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConstIteratorType end() const noexcept;
		///@}
		// NOLINTEND(readability-identifier-naming)

		private:
		constexpr void DoInitialize(SizeType initial_capacity) noexcept;
		constexpr void DoEnsureCapacity(SizeType requested_capacity) noexcept;
		constexpr ReferenceType DoAdd(KeyType&& key, ValueType&& value) noexcept;
		constexpr SizeType DoFindSlot(KeyType&& key) noexcept;
	};

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(SizeType initial_buckets,
	                                                                   const AllocatorType& allocator)
		: _allocator(allocator)
	{
		DoInitialize(initial_buckets);
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(std::initializer_list<ValueType> list) noexcept
	{
		DoInitialize(list.size());
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(const HashMap& other) noexcept
	{
		if (&other == this) {
			return;
		}
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(HashMap&& other) noexcept
	{
		if (&other == this) {
			return;
		}
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::operator=(const HashMap& other) noexcept -> HashMap&
	{
		if (&other == this) {
			return *this;
		}
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::operator=(HashMap&& other) noexcept -> HashMap&
	{
		ORION_IGNORE_PARAM(other);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::operator=(
		std::initializer_list<ValueType> list) noexcept -> HashMap&
	{
		DoEnsureCapacity(list.size());
		for (auto& [key, value] : list) {
			Add(key, value);
		}
		return *this;
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Add(
		const KeyType& key,
		const ValueType& value) noexcept -> ReferenceType
	{
		return DoAdd(key, value);
	}
	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Add(const KeyType& key,
	                                                                                       ValueType&& value) noexcept
		-> ValueType&
	{
		return DoAdd(key, Move(value));
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Add(
		KeyType&& key,
		const ValueType& value) noexcept -> ReferenceType
	{
		return DoAdd(Move(key), value);
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Add(KeyType&& key,
	                                                                                       ValueType&& value) noexcept
		-> ReferenceType
	{
		return DoAdd(Move(key), Move(value));
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Remove(const KeyType& key) noexcept -> void
	{
		DoRemove(key);
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Remove(KeyType&& key) noexcept -> void
	{
		DoRemove(Move(key));
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Find(const KeyType& key) noexcept
		-> PointerType
	{
		SizeType slot_index = DoFindSlot(key);
		return _data[slot_index].state == Detail::HashMapElementState::Allocated ? _data[slot_index].value : nullptr;
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Find(
		const KeyType& key) const noexcept -> ConstPointerType
	{
		SizeType slot_index = DoFindSlot(key);
		return _data[slot_index].state == Detail::HashMapElementState::Allocated ? _data[slot_index].value : nullptr;
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Contains(const KeyType& key) const noexcept -> Bool8
	{
		ORION_IGNORE_PARAM(key);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Clear() noexcept -> void
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::IsEmpty() const noexcept -> Bool8
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Size() const noexcept -> SizeType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::ByteSize() const noexcept
		-> SizeType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Capacity() const noexcept
		-> SizeType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::begin() noexcept -> IteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::begin() const noexcept
		-> ConstIteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::end() noexcept -> IteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::end() const noexcept
		-> ConstIteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoInitialize(SizeType initial_capacity) noexcept
		-> void
	{
		initial_capacity       = ORION_MAX(initial_capacity, k_initial_bucket_count);
		SizeType size_in_bytes = sizeof(ValueType);
		SizeType alignment     = alignof(ValueType);
		_data                  = static_cast<ElementType*>(_allocator.Allocate(size_in_bytes, alignment));
		_capacity              = initial_capacity;
		_size                  = 0;
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoEnsureCapacity(
		SizeType requested_capacity) noexcept -> void
	{
		ORION_IGNORE_PARAM(requested_capacity);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoAdd(KeyType&& key, ValueType&& value) noexcept
		-> ReferenceType
	{
		ORION_IGNORE_PARAM(key);
		ORION_IGNORE_PARAM(value);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoFindSlot(KeyType&& key) noexcept -> SizeType
	{
		SizeType slot_index = Hash(key) % _capacity;
		while (_data[slot_index].state == Detail::HashMapElementState::Allocated
		       && !Predicate(_data[slot_index].key, key)) {
			slot_index = (slot_index + 1) % _capacity;
		}
		return slot_index;
	}
}  // namespace Orion::Engine
