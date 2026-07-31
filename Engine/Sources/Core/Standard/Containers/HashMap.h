#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Memory/Allocators/PlatformAllocator.h"
#include "Core/Standard/Algorithms/Compare.h"
#include "Core/Standard/Algorithms/Hash.h"
#include "Core/Standard/Containers/Pair.h"
#include "Core/Standard/Memory/Lifetime.h"
#include "Core/Standard/TypeTraits.h"
#include "Core/Standard/Utility/MathUtils.h"

#include <initializer_list>

namespace Orion::Engine
{
	namespace Detail
	{

		/// @brief Actual storage type for a single HashMap's key-value pair.
		template <typename T1, typename T2>
		struct HashMapElement
		{
			T1 key;
			T2 value;
			enum class State : UInt8
			{
				Free,
				Allocated,
			} state;
		};
	}  // namespace Detail

	/**
	 * @brief Represents a type-safe container capable of holding key-value pairs with unique keys.
	 *
	 * @warning
	 *
	 * @tparam Key Type of the key in the key-value pair.
	 * @tparam Value Type of the value in the key-value pair.
	 * @tparam Hash TODO
	 * @tparam Predicate TODO
	 * @tparam Allocator Allocator to be used with the HashMap that will perform all the allocations.
	 */
	template <typename Key,
	          typename Value,
	          typename Hash      = Algorithm::Hash<Key>,
	          auto Predicate     = Algorithm::Equal<Key>,
	          typename Allocator = PlatformAllocator>
	class HashMap
	{
		private:
		using StorageType = Detail::HashMapElement<Key, Value>;

		public:
		using ThisType           = HashMap;
		using KeyType            = Key;
		using ValueType          = Value;
		using KeyValueType       = Pair<Key, Value>;
		using SizeType           = USize;
		using HashType           = Hash;
		using AllocatorType      = Allocator;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;

		using IteratorType      = PointerType;       // TODO Iterators
		using ConstIteratorType = ConstPointerType;  // TODO Iterators

		static constexpr SizeType k_initial_bucket_count = 16UL;
		static constexpr Float64 k_desired_load_factor   = 0.8;

		private:
		AllocatorType _allocator{};
		StorageType* _data{ nullptr };
		SizeType _capacity{ 0UL };
		SizeType _size{ 0UL };

		public:
		constexpr explicit HashMap(SizeType initial_buckets       = k_initial_bucket_count,
		                           const AllocatorType& allocator = AllocatorType());
		constexpr HashMap(std::initializer_list<KeyValueType>) noexcept;
		constexpr HashMap(const HashMap&) noexcept;
		constexpr HashMap(HashMap&&) noexcept;
		constexpr ~HashMap();

		constexpr HashMap& operator=(const HashMap&) noexcept;
		constexpr HashMap& operator=(HashMap&&) noexcept;
		constexpr HashMap& operator=(std::initializer_list<ValueType>) noexcept;

		/** Inserts new element to the hashmap. If the key already exists, it will be overridden.  */
		///@{
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Insert(const KeyType& key,
		                                                                const ValueType& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Insert(const KeyType& key, ValueType&& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Insert(KeyType&& key, const ValueType& value) noexcept;
		[[nodiscard]] ORION_FORCE_INLINE constexpr ReferenceType Insert(KeyType&& key, ValueType&& value) noexcept;
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

		/// @brief Verifies that there exists an entry for a given \p key.
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

		/// @brief TODO
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 LoadFactor() const noexcept;

		/// @brief TODO
		[[nodiscard]] ORION_FORCE_INLINE constexpr Float64 MaxLoadFactor() const noexcept;

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
		constexpr void DoRebuildHashMapIfNeeded() noexcept;
		constexpr ReferenceType DoInsert(KeyValueType&& element) noexcept;
		constexpr void DoRemove(KeyType&& key) noexcept;
		constexpr SizeType DoFindSlot(const KeyType& key) const noexcept;
		constexpr void DoSwap(ThisType& other) noexcept;
	};

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(SizeType initial_buckets,
	                                                                   const AllocatorType& allocator)
		: _allocator(allocator)
	{
		DoInitialize(initial_buckets);
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(
		std::initializer_list<KeyValueType> list) noexcept
		: _allocator(AllocatorType{})
	{
		DoInitialize(list.size());
		for (auto& it : list) {
			DoInsert({ it.first, it.second });
		}
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(const HashMap& other) noexcept
		: _allocator(other._allocator), _capacity(other._capacity), _size(other._size)
	{
		SizeType size_in_bytes = sizeof(StorageType) * _capacity;
		_data                  = static_cast<StorageType*>(_allocator.Allocate(size_in_bytes, alignof(StorageType)));
		if constexpr (IsTriviallyCopyable<StorageType>) {
			Platform::MemoryCopy(_data, other._data, size_in_bytes);
		} else {
			Memory::ConstructItems(_data, other._data, _capacity);
		}
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::HashMap(HashMap&& other) noexcept
	{
		DoSwap(other);
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr HashMap<Key, Value, Hash, Predicate, Allocator>::~HashMap()
	{
		if (_data) {
			Memory::DestructItems(_data, _capacity);
			_allocator.Free(_data);
		}
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::operator=(const HashMap& other) noexcept -> HashMap&
	{
		if (this == &other) {
			return *this;
		}

		if (_data) {
			Memory::DestructItems(_data, _capacity);
			_allocator.Free(_data);
		}

		_allocator = other._allocator;
		_capacity  = other._capacity;
		_size      = other._size;
		_data = static_cast<StorageType*>(_allocator.Allocate(sizeof(StorageType) * _capacity, alignof(StorageType)));

		if constexpr (IsTriviallyCopyable<ValueType>) {
			Platform::MemoryCopy(_data, other._data, sizeof(StorageType) * _capacity);
		} else {
			Memory::ConstructItems(_data, other._data, _capacity);
		}

		return *this;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::operator=(HashMap&& other) noexcept -> HashMap&
	{
		if (this == &other) {
			return *this;
		}

		DoSwap(other);

		return *this;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::operator=(
		std::initializer_list<ValueType> list) noexcept -> HashMap&
	{
		ORION_IGNORE_PARAM(list);
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Insert(
		const KeyType& key,
		const ValueType& value) noexcept -> ReferenceType
	{
		return DoInsert({ key, value });
	}
	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Insert(
		const KeyType& key,
		ValueType&& value) noexcept -> ValueType&
	{
		return DoInsert({ key, Move(value) });
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Insert(
		KeyType&& key,
		const ValueType& value) noexcept -> ReferenceType
	{
		return DoInsert({ Move(key), value });
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Insert(
		KeyType&& key,
		ValueType&& value) noexcept -> ReferenceType
	{
		return DoInsert({ Move(key), Move(value) });
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Remove(const KeyType& key) noexcept -> void
	{
		DoRemove(key);
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Remove(KeyType&& key) noexcept -> void
	{
		DoRemove(Move(key));
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Find(const KeyType& key) noexcept
		-> PointerType
	{
		ORION_ASSERT_DEBUG(_data);
		SizeType slot_index = DoFindSlot(key);
		StorageType element = _data[slot_index];
		return element.state == StorageType::State::Allocated ? &element.value : nullptr;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Find(
		const KeyType& key) const noexcept -> ConstPointerType
	{
		ORION_ASSERT_DEBUG(_data);
		SizeType slot_index = DoFindSlot(key);
		StorageType element = _data[slot_index];
		return element.state == StorageType::State::Allocated ? &element.value : nullptr;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Contains(const KeyType& key) const noexcept -> Bool8
	{
		SizeType slot_index = DoFindSlot(key);
		return _data[slot_index].state == StorageType::State::Allocated;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Clear() noexcept -> void
	{
		for (SizeType index = 0; index < _capacity; ++index) {
			if constexpr (!IsTriviallyDestructible<KeyType>) {
				_data[index].key.~KeyType();
			}
			if constexpr (!IsTriviallyDestructible<ValueType>) {
				_data[index].value.~ValueType();
			}
			_data[index].state = StorageType::State::Free;
		}
		_size = 0;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::IsEmpty() const noexcept -> Bool8
	{
		return _size == 0UL;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Size() const noexcept -> SizeType
	{
		return _size;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::ByteSize() const noexcept
		-> SizeType
	{
		return _size * sizeof(KeyValueType);
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Capacity() const noexcept
		-> SizeType
	{
		return _capacity;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::LoadFactor() const noexcept
		-> Float64
	{
		return static_cast<Float64>(_size) / static_cast<Float64>(_capacity);
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::MaxLoadFactor() const noexcept
		-> Float64
	{
		return k_desired_load_factor;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::begin() noexcept -> IteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::begin() const noexcept
		-> ConstIteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::end() noexcept -> IteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	ORION_FORCE_INLINE constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::end() const noexcept
		-> ConstIteratorType
	{
		ORION_NOT_IMPLEMENTED();
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoInitialize(SizeType initial_capacity) noexcept
		-> void
	{
		initial_capacity       = ORION_MAX(initial_capacity, k_initial_bucket_count);
		SizeType size_in_bytes = sizeof(StorageType) * initial_capacity;
		SizeType alignment     = alignof(StorageType);
		_data                  = static_cast<StorageType*>(_allocator.Allocate(size_in_bytes, alignment));
		_capacity              = initial_capacity;
		_size                  = 0;

		if (_data) {
			for (SizeType index = 0; index < _capacity; ++index) {
				_data[index].state = StorageType::State::Free;
			}
		}
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoRebuildHashMapIfNeeded() noexcept -> void
	{
		ORION_ASSERT_DEBUG(_data);
		if (LoadFactor() < MaxLoadFactor()) {
			return;
		}

		SizeType new_capacity  = ToNextPowerOfTwo(_capacity + 1);
		SizeType size_in_bytes = sizeof(StorageType) * new_capacity;
		StorageType* new_data  = static_cast<StorageType*>(_allocator.Allocate(size_in_bytes, alignof(StorageType)));
		ORION_ASSERT_DEBUG(new_data);

		if (_capacity > 0) {
			if constexpr (IsTriviallyCopyable<StorageType>) {
				SizeType previous_size_in_bytes = sizeof(StorageType) * _capacity;
				Platform::MemoryCopy(new_data, _data, previous_size_in_bytes);
			} else {
				for (SizeType index = 0; index < _capacity; ++index) {
					Memory::ConstructItem(&new_data[index], Move(_data[index]));
					_data[index].~StorageType();
				}
			}
		}

		_allocator.Free(_data);
		_data     = new_data;
		_capacity = new_capacity;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoInsert(KeyValueType&& element) noexcept
		-> ReferenceType
	{
		ORION_ASSERT_DEBUG(_data);
		const auto& [key, value] = element;
		SizeType slot_index      = DoFindSlot(key);
		if (_data[slot_index].state == StorageType::State::Allocated) {
			_data[slot_index].value = value;
			return _data[slot_index].value;
		}

		DoRebuildHashMapIfNeeded();
		slot_index              = DoFindSlot(key);
		_data[slot_index].key   = key;
		_data[slot_index].value = value;
		_data[slot_index].state = StorageType::State::Allocated;
		++_size;
		return _data[slot_index].value;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoRemove(KeyType&& key) noexcept -> void
	{
		ORION_ASSERT_DEBUG(_data);
		SizeType slot_index = DoFindSlot(key);
		if (_data[slot_index].state == StorageType::State::Free) {
			return;
		}

		_data[slot_index].state = StorageType::State::Free;

		SizeType index = slot_index;
		while (true) {
			index = (index + 1) % _capacity;
			if (_data[index].state == StorageType::State::Free) {
				break;
			}

			typename HashType::SizeType hash = HashType()(_data[index].key) % _capacity;
			if (slot_index <= index) {
				if (slot_index < hash && hash <= index) {
					continue;
				}
			} else {
				if (hash <= index || slot_index <= hash) {
					continue;
				}
			}

			Memory::DestructItems(&_data[slot_index], 1);
			_data[slot_index].state = StorageType::State::Allocated;
			_data[index].state      = StorageType::State::Free;
			_data[slot_index].key   = _data[index].key;
			_data[slot_index].value = _data[index].value;
			--_size;

			slot_index = index;
		}
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoFindSlot(const KeyType& key) const noexcept
		-> SizeType
	{
		typename HashType::SizeType index = HashType()(key) % _capacity;
		while (_data[index].state == StorageType::State::Allocated && !Predicate(_data[index].key, key)) {
			index = (index + 1) % _capacity;
		}
		return index;
	}

	template <typename Key, typename Value, typename Hash, auto Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::DoSwap(ThisType& other) noexcept -> void
	{
		Swap(_allocator, other._allocator);
		Swap(_data, other._data);
		Swap(_capacity, other._capacity);
		Swap(_size, other._size);
	}
}  // namespace Orion::Engine
