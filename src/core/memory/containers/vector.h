#pragma once

#include "platform/compiler_macros.h"
#include "platform/memory.h"
#include "platform/types.h"
#include "core/assert.h"
#include "util/move.h"
#include "orion_config.h"

// TODO(sand_noodles): This class should use allocators.
// TODO(sand_noodles): Capacity of the vector should be aligned to a power of two?
namespace orion
{
	/**
	 * @brief Type safe implementation of dynamic arrays.
	 * @tparam T type of the variable stored.
	 */
	template <typename T>
	struct vector_t
	{
		public:
			typedef T                 value_type;
			typedef T*                pointer;
			typedef const T*          const_pointer;
			typedef T&                reference;
			typedef const T&          const_reference;
			typedef T*                iterator;
			typedef const T*          const_iterator;
			typedef usize             size_type;

		private:
			pointer      _data;
			size_type    _size;
			size_type    _capacity;

		public:
			/**
			 * @brief Creates new empty vector with given initial capacity.
			 * @param initial_capacity initial capacity of the vector.
			 */
			static vector_t create(size_type initial_capacity = ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY);

			/** Destroys the given vector; freeing all of its allocated memory. */
			void destroy();

			ORION_CONSTEXPR reference       operator[](usize index);
			ORION_CONSTEXPR const_reference operator[](usize index) const;

			ORION_CONSTEXPR reference       front();
			ORION_CONSTEXPR const_reference front() const;
			ORION_CONSTEXPR reference       back();
			ORION_CONSTEXPR const_reference back() const;
			ORION_CONSTEXPR pointer         data();
			ORION_CONSTEXPR const_pointer   data() const;

			ORION_CONSTEXPR iterator       begin();
			ORION_CONSTEXPR const_iterator begin() const;
			ORION_CONSTEXPR iterator       end();
			ORION_CONSTEXPR const_iterator end() const;

			[[nodiscard]] ORION_CONSTEXPR b8        empty() const;
			[[nodiscard]] ORION_CONSTEXPR size_type size() const;
			[[nodiscard]] ORION_CONSTEXPR size_type capacity() const;
			ORION_CONSTEXPR void                    reserve(size_type capacity);
			ORION_CONSTEXPR void                    shrink_to_fit();

			ORION_CONSTEXPR void push_back(const_reference value);
			ORION_CONSTEXPR void push_back(T&& value);
			ORION_CONSTEXPR void pop_back();
			ORION_CONSTEXPR void clear();

		private:
			void ensure_capacity();
	};

	template <typename T>
	auto vector_t<T>::create(size_type initial_capacity) -> vector_t
	{
		vector_t<T> v;
		v._size     = 0;
		v._capacity =
			initial_capacity > ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY
			? initial_capacity
			: ORION_CONTAINERS_VECTOR_INITIAL_CAPACITY;
		v._data     = (pointer)platform_allocate(v._capacity * sizeof(value_type));
		return v;
	}

	template <typename T>
	auto vector_t<T>::destroy() -> void
	{
		platform_free(_data);
		_data     = nullptr;
		_size     = 0;
		_capacity = 0;
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::operator[](usize index) -> reference
	{
		return _data[index];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::operator[](usize index) const -> const_reference
	{
		return _data[index];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::front() -> reference
	{
		return _data[0];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::front() const -> const_reference
	{
		return _data[0];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::back() -> reference
	{
		return _data[_size - 1];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::back() const -> const_reference
	{
		return _data[_size - 1];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::data() -> pointer
	{
		return _data;
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::data() const -> const_pointer
	{
		return _data;
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::begin() -> iterator
	{
		return _data[0];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::begin() const -> const_iterator
	{
		return _data[0];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::end() -> iterator
	{
		return _data[_size - 1];
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::end() const -> const_iterator
	{
		return _data[_size - 1];
	}

	template <typename T>
	[[nodiscard]] ORION_CONSTEXPR auto vector_t<T>::empty() const -> b8
	{
		return _size == 0;
	}

	template <typename T>
	[[nodiscard]] ORION_CONSTEXPR auto vector_t<T>::size() const -> size_type
	{
		return _size;
	}

	template <typename T>
	[[nodiscard]] ORION_CONSTEXPR auto vector_t<T>::capacity() const -> size_type
	{
		return _capacity;
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::reserve(size_type capacity) -> void
	{
		if(capacity < _capacity)
			return;
		_data = (pointer)platform_reallocate(_data, capacity * sizeof(value_type));
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::shrink_to_fit() -> void
	{
		_data = (pointer)platform_reallocate(_data, _size * sizeof(value_type));
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::push_back(const_reference value) -> void
	{
		ensure_capacity();
		_data[_size++] = value;
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::push_back(T&& value) -> void
	{
		ensure_capacity();
		_data[_size++] = move(value);
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::pop_back() -> void
	{
		if(_size > 0)
			_size--;
	}

	template <typename T>
	ORION_CONSTEXPR auto vector_t<T>::clear() -> void
	{
		// Cleaning up after an object is not our job - just zero the size.
		_size = 0;
	}

	//
	// Private
	//

	template <typename T>
	auto vector_t<T>::ensure_capacity() -> void
	{
		if(_size + 1 > _capacity)
		{
			const size_type new_capacity = _capacity * ORION_CONTAINERS_VECTOR_CAPACITY_GROWTH_RATE;
			_data     = (pointer)platform_reallocate(_data, new_capacity * sizeof(value_type));
			_capacity = new_capacity;
			OE_ASSERT_TRUE(_data, "allocator_t: failed to allocate new memory.");
		}
	}
}  // namespace orion
