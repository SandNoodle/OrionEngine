#pragma once

#include "platform/types.h"
#include "platform/compiler_macros.h"

#include "core/math/omath.h"

namespace orion
{
	/**
	 * @brief Structure representing a 4x4 Matrix.
	 * @tparam T numeric type.
	 */
	template <typename T>
	struct mat4
	{
		typedef mat4<T>  this_type;
		typedef T        value_type;
		typedef T&       reference;
		typedef const T& const_reference;
		typedef usize    size_type;

		value_type data[16];

		/** Returns 4x4 identity matrix. */
		ORION_CONSTEXPR static this_type identity();

		/**
		 * @brief Returns Perspective projection matrix.
		 * @param fov Camera's field of view.
		 * @param aspect_ratio Aspect ratio of the view window. Typically height / width.
		 * @param near_plane Distance to a plane closest to the camera.
		 * @param far_plane Distance to a plane furthest to the camera.
		 */
		ORION_CONSTEXPR static this_type perspective(value_type fov,
			                                         value_type aspect_ratio,
			                                         value_type near_plane,
			                                         value_type far_plane);

		/**
		 * @brief Returns Orthographic projection matrix.
		 * @param left Point furthest to the left. Typically 0.
		 * @param right Point furthest to the right. Typically width.
		 * @param top Point furthest to the top. Typically height;
		 * @param bottom Point furthest to the bottom. Typically 0.
		 * @param near_plane Distance to a plane closest to the camera.
		 * @param far_plane Distance to a plane furthest to the camera.
		 */
		ORION_CONSTEXPR static this_type orthographic(value_type left,
			                                          value_type right,
			                                          value_type top,
			                                          value_type bottom,
			                                          value_type near_plane,
			                                          value_type far_plane);

		/** Returns transposed copy of this matrix (rows -> columns). */
		ORION_CONSTEXPR const this_type& transpose() const;

		ORION_CONSTEXPR reference       operator[](size_type index);
		ORION_CONSTEXPR const_reference operator[](size_type index) const;
		ORION_CONSTEXPR this_type       operator*(const this_type& other);
		ORION_CONSTEXPR this_type&      operator*=(const this_type& other);
		ORION_CONSTEXPR b8              operator==(const this_type& other) const;
	};

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::identity() -> this_type
	{
		mat4<T> m = {0};
		m[0]  = 1;
		m[5]  = 1;
		m[10] = 1;
		m[15] = 1;
		return m;
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::perspective(value_type fov,
		                                      value_type aspect_ratio,
		                                      value_type near_plane,
		                                      value_type far_plane) -> this_type
	{
		this_type m = {0};
		value_type scale = 0.5 * math::tan(fov * 0.5);
		m[0]  = scale;
		m[5]  = scale;
		m[10] = -far_plane / (far_plane - near_plane);
		m[11] = (-far_plane * near_plane) / (far_plane - near_plane);
		m[14] = -1.0f;
		return m;
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::orthographic(value_type left,
		                                       value_type right,
		                                       value_type top,
		                                       value_type bottom,
		                                       value_type near_plane,
		                                       value_type far_plane) -> this_type
	{
		this_type m = this_type::identity();
		m.data[0]  = 0.5f * (right - left);
		m.data[5]  = 0.5f * (top - bottom);
		m.data[10] = -0.5f * (far_plane - near_plane);
		m.data[12] = -((right + left) / (right - left));
		m.data[13] = -((top + bottom) / (top - bottom));
		m.data[14] = -((far_plane + near_plane) / (far_plane - near_plane));
		return m;
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::transpose() const -> const this_type&
	{
		this[0]  = this[0];
		this[1]  = this[4];
		this[2]  = this[8];
		this[3]  = this[12];
		this[4]  = this[1];
		this[5]  = this[5];
		this[6]  = this[9];
		this[7]  = this[13];
		this[8]  = this[2];
		this[9]  = this[6];
		this[10] = this[10];
		this[11] = this[14];
		this[12] = this[3];
		this[13] = this[7];
		this[14] = this[11];
		this[15] = this[15];
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::operator[](size_type index) -> reference
	{
		return data[index];
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::operator[](size_type index) const -> const_reference
	{
		return data[index];
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::operator*(const this_type& other) -> this_type
	{
		return *this *= other;
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::operator*=(const this_type& other) -> this_type&
	{
		for(size_t column = 0; column < 4; ++column)
		{
			for(size_t row = 0; row < 4; ++row)
			{
				this[row + column * 4] += this[row + 0 * 4] * other[column + 0 * 4];
				this[row + column * 4] += this[row + 1 * 4] * other[column + 1 * 4];
				this[row + column * 4] += this[row + 2 * 4] * other[column + 2 * 4];
				this[row + column * 4] += this[row + 3 * 4] * other[column + 3 * 4];
			}
		}
		return *this;
	}

	template <typename T>
	ORION_CONSTEXPR auto mat4<T>::operator==(const this_type& other) const -> b8
	{
		for(size_type i = 0; i < 16; ++i)
		{
			if (!math::compare(this[i], other[i]))
			{
				return false;
			}
		}
		return true;
	}
}
