#pragma once

namespace orion::util
{
	/**
	 * @brief Provides member typedef to a type referenced by T.
	 * @tparam T Type refered by T or T if it is not a reference.
	 */
	template <class T> struct remove_reference      { typedef T type; };
	template <class T> struct remove_reference<T&>  { typedef T type; };
	template <class T> struct remove_reference<T&&> { typedef T type; };

}  // namespace orion::util
 
