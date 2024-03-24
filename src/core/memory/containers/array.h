#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"

namespace orion
{
	/**
	 * @brief Type-safe thin wrapper around C style arrays.
	 * @tparam T type of the variable stored.
	 * @tparam N maximum number of elements to be stored.
	 */
	template<typename T, usize N>
	struct array_t
	{
		public:
			typedef T        value_type;
			typedef T*       pointer;
			typedef const T* const_pointer;
			typedef T&       reference;
			typedef const T& const_reference;
			typedef T*       iterator;
			typedef const T* const_iterator;
			typedef usize    size_type;

		private:
			T _data[N];

		public:
			T&       operator[](usize index);
			const T& operator[](usize index) const;

			ORION_CONSTEXPR reference       front();
			ORION_CONSTEXPR const_reference front() const;
			ORION_CONSTEXPR reference       back();
			ORION_CONSTEXPR const_reference back() const;
			ORION_CONSTEXPR pointer         data();
			ORION_CONSTEXPR const_pointer   data() const;

			// @TODO Begin / End interators.

			[[nodiscard]] ORION_CONSTEXPR b8        empty() const noexcept;
			[[nodiscard]] ORION_CONSTEXPR size_type size() const noexcept;

			// @TODO <=> operator
	};

	template<typename T, usize N>
	auto array_t<T, N>::operator[](size_type index) -> reference
	{
		return _data[index];
	}

	template<typename T, usize N>
	auto array_t<T, N>::operator[](size_type index) const -> const_reference
	{
		return _data[index];
	}

	template<typename T, usize N>
	ORION_CONSTEXPR auto array_t<T, N>::front() -> reference
	{
		return _data[0];
	}


	template<typename T, usize N>
	ORION_CONSTEXPR auto array_t<T, N>::front() const -> const_reference
	{
		return _data[0];
	}

	template<typename T, usize N>
	ORION_CONSTEXPR auto array_t<T, N>::back() -> reference
	{
		return _data[N - 1];
	}


	template<typename T, usize N>
	ORION_CONSTEXPR auto array_t<T, N>::back() const -> const_reference
	{
		return _data[N - 1];
	}

	template<typename T, usize N>
	ORION_CONSTEXPR auto array_t<T, N>::data() -> pointer
	{
		return _data;
	}

	template<typename T, usize N>
	ORION_CONSTEXPR auto array_t<T, N>::data() const -> const_pointer
	{
		return _data;
	}

	template<typename T, usize N>
	[[nodiscard]] ORION_CONSTEXPR auto array_t<T, N>::empty() const noexcept -> b8
	{
		return N == 0;
	}

	template<typename T, usize N>
	[[nodiscard]] ORION_CONSTEXPR auto array_t<T, N>::size() const noexcept -> size_type
	{
		return N;
	}
}  // namespace orion
