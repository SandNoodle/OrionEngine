#pragma once

#include "core/memory/allocators/allocator.h"

namespace orion
{
	/**
	 * @brief Thin, type-safe wrapper for allocator_t.
	 * @tparam T
	 */
	template <typename T>
	struct typed_allocator_t
	{
		typedef T                      value_type;
		typedef T*                     pointer;
		typedef allocator_t::size_type size_type;

		allocator_t _allocator;

		/**
		 * @brief
		 * @param allocator
		 * @return
		 */
		static typed_allocator_t create(allocator_t allocator);

		/**
		 * @brief
		 * @param initial_capacity
		 */
		void create(size_type initial_capacity);

		/**
		 * @brief
		 */
		void destroy();

		/**
		 * @brief
		 * @param count
		 * @return
		 */
		pointer allocate(size_type count);

		/**
		 * @brief
		 * @param count
		 * @param alignment
		 * @return
		 */
		pointer allocate_aligned(size_type count, size_type alignment);

		/**
		 * @brief
		 * @param ptr
		 */
		void deallocate(pointer ptr);

		/**
		 * @brief
		 */
		void deallocate_all();

		/** @brief */
		size_type size();

		/** @brief */
		size_type capacity();
	};


	template <typename T>
	auto typed_allocator_t<T>::create(allocator_t allocator) -> typed_allocator_t<value_type>
	{
		return (typed_allocator_t<value_type>) {
			._allocator = allocator,
		};
	}

	template <typename T>
	auto typed_allocator_t<T>::create(size_type initial_capacity) -> void
	{
		_allocator.create(initial_capacity * sizeof(value_type),
			              _allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::destroy() -> void
	{
		_allocator.destroy(_allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::allocate(size_type size) -> pointer
	{
		return (pointer)_allocator.allocate(sizeof(value_type) * size,
			                                _allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::allocate_aligned(size_type size, size_type alignment) -> pointer
	{
		return (pointer)_allocator.allocate_aligned(sizeof(value_type) * size,
			                                        alignment,
			                                        _allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::deallocate(pointer ptr) -> void
	{
		_allocator.deallocate(ptr, sizeof(value_type), _allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::deallocate_all() -> void
	{
		_allocator.deallocate_all(_allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::size() -> size_type
	{
		return _allocator.size(_allocator.user_context);
	}

	template <typename T>
	auto typed_allocator_t<T>::capacity() -> size_type
	{
		return _allocator.capacity(_allocator.user_context);
	}
}
