#pragma once

#include "OrionEngine.h"

#include "Core/Standard/TypeTraits.h"

namespace Orion::Engine
{
	/**
	 * @brief Constructs \p count elements into a memory, effectively begins C++'s lifetime for these elements.
	 *
	 * @param dst Destination address to where copy the elements.
	 * @param src Source address of the first element to pass.
	 * @param count Number of elements to copy from \p src.
	 */
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
		requires(sizeof(SourceItemType) > 0 && sizeof(DestinationItemType) > 0)
	ORION_FORCE_INLINE void ConstructItems(DestinationItemType* dst, const SourceItemType* src, SizeType count)
	{
		while (count--) {
			::new ((void*)dst) DestinationItemType(*src);
			++src;
			++dst;
		}
	}

	/**
	 * @brief Constructs a given element into a memory, effectively begins C++'s lifetime for it.
	 * @param dst Destination address where the element will be constructed.
	 * @param item Element to construct.
	 */
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
	ORION_FORCE_INLINE void ConstructItem(DestinationItemType* dst, const SourceItemType& item)
	{
		ConstructItems(dst, &item, 1);
	}

	/**
	 * @brief Constructs a given element into a memory, effectively begins C++'s lifetime for it.
	 * @param dst Destination address where the element will be constructed.
	 * @param item Element to construct.
	 */
	template <typename SourceItemType, typename DestinationItemType = SourceItemType, typename SizeType = USize>
	ORION_FORCE_INLINE void ConstructItem(DestinationItemType* dst, SourceItemType&& item)
	{
		ConstructItems(dst, &item, 1);
	}

	/**
	 * @brief TODO
	 * @tparam ItemType
	 * @tparam SizeType
	 * @param src
	 * @param count
	 */
	template <typename ItemType, typename SizeType>
		requires(sizeof(ItemType) > 0)
	ORION_FORCE_INLINE void DestructItems(ItemType* src, SizeType count)
	{
		if (!IsTriviallyDestructible<ItemType>) {
			while (count--) {
				src->ItemType::~ItemType();
			}
		}
	}
}  // namespace Orion::Engine
