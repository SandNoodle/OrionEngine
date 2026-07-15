#pragma once

#include "OrionEngine.h"

#include "Core/Memory/Allocators/PlatformAllocator.h"

namespace Orion::Engine
{
	namespace Detail
	{
		template <typename CharT, typename Allocator = PlatformAllocator>
		class StringBase
		{
			public:
			using ThisType         = StringBase<CharT>;
			using ValueType        = CharT;
			using SizeType         = USize;
			using AllocatorType    = Allocator;
			using PointerType      = ValueType*;
			using ConstPointerType = const ValueType*;
			using HashType         = UInt64;

			private:
			AllocatorType _allocator;
			union
			{
				struct
				{
					PointerType _data;
					SizeType _size;
				} _heap_data;
				ValueType _stack_data[sizeof(_heap_data)];
			};
			SizeType _capacity;

			public:
			constexpr explicit StringBase() = default;
		};
	}  // namespace Detail

	using String = Detail::StringBase<Char8>;

}  // namespace Orion::Engine
