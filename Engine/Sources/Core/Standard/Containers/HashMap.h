#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Memory/Allocators/PlatformAllocator.h"

namespace Orion::Engine
{
	template <typename T>
	struct Hash;

	template <typename T>
	struct Predicate;

	/**
	 * @brief TODO
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
		using KeyType   = Key;
		using ValueType = Value;
		using SizeType  = USize;

		private:
		public:
		[[nodiscard]] constexpr Bool8 Contains(const KeyType& key) const noexcept;
	};

	template <typename Key, typename Value, typename Hash, typename Predicate, typename Allocator>
	constexpr auto HashMap<Key, Value, Hash, Predicate, Allocator>::Contains(const KeyType& key) const noexcept -> Bool8
	{
		ORION_IGNORE_PARAM(key);
		ORION_NOT_IMPLEMENTED();
	}
}  // namespace Orion::Engine
