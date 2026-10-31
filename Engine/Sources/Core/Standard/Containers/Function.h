#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Standard/Math/Math.h"
#include "Core/Standard/Memory/Lifetime.h"
#include "Core/Standard/Traits/TypeComparison.h"
#include "Core/Standard/Traits/TypeQualifiers.h"
#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine
{
	template <typename Signature, USize StorageSize = ORION_CONTAINERS_FUNCTION_DEFAULT_STORAGE_SIZE>
	class Function;

	/// @brief Concept that is satisfied by a callable types. Example(s): function pointers, member functions, etc.
	template <typename T, USize StorageSize, typename R, typename... Args>
	concept FunctionCallableKind = !IsSame<Decay<T>, Function<R(Args...), StorageSize>>;

	namespace Detail
	{
		/// @brief Represents a helper structure, that guarantees the correct alignment of the stored callable.
		/// @details Inspired by eastl::function's implementation.
		union FunctionStorageAlignmentHelper
		{
			// clang-format off
			class FunctionStorageAlignmentDummy {};
			// clang-format on

			void (*unused_fn)();
			void (FunctionStorageAlignmentDummy::*unused_member_fn)();
			void* unused_ptr;
			Int64 unused_var;
		};

		/// @brief Represents the actual (inline) storage for the type-erased callable function/object.
		/// @tparam StorageSize Amount of bytes capable of storing the callable.
		template <USize StorageSize = ORION_CONTAINERS_FUNCTION_DEFAULT_STORAGE_SIZE>
		struct FunctionStorage
		{
			public:
			using SizeType      = USize;
			using AlignmentType = FunctionStorageAlignmentHelper;

			static constexpr SizeType k_inline_alignment = Math::Max(alignof(AlignmentType), sizeof(AlignmentType));
			static constexpr SizeType k_inline_size      = Math::Max(StorageSize, sizeof(AlignmentType));

			public:
			alignas(k_inline_alignment) Byte data[k_inline_size];
		};

		/// @brief Represents a type-erasure interface for storing the callable objects.
		template <typename R, typename... Args>
		struct IFunctionCallable
		{
			public:
			using ThisType   = IFunctionCallable;
			using ReturnType = R;

			public:
			virtual ~IFunctionCallable() = default;

			/// @brief Calls the stored callable function/object in a type-safe manner.
			/// @param[IN, REQUIRED] args Arguments matching the signature of the stored callable.
			virtual ReturnType Call(Args... args) noexcept = 0;
		};

		/// @brief Represents type-aware storage of the callable pointer.
		template <typename Functor, typename R, typename... Args>
		struct FunctionCallable : IFunctionCallable<R, Args...>
		{
			public:
			using BaseType    = IFunctionCallable<R, Args...>;
			using ThisType    = FunctionCallable;
			using FunctorType = Functor;
			using ReturnType  = R;

			public:
			FunctorType fn;

			public:
			template <typename Fn>
			explicit FunctionCallable(Fn&& fn);
			~FunctionCallable() override = default;

			ReturnType Call(Args... args) noexcept override;
		};
	}  // namespace Detail

	/// @brief Represents a general-purpose function wrapper capable of storing any callable object (functions, member
	/// functions, etc.) in a typesafe manner.
	/// @warning This implementation does NOT allocate memory on the heap, i.e., storage is fully inlined on the
	/// stack - meaning, that some capturing lambdas may exceed the StorageLimit that was put in place. In that case,
	/// you have to evaluate if you want to remove some captures or increase the StorageSize passed.
	template <typename R, typename... Args, USize StorageSize>
	class Function<R(Args...), StorageSize>
	{
		public:
		using ThisType   = Function;
		using ReturnType = R;
		using SizeType   = USize;

		private:
		using StorageType  = Detail::FunctionStorage<StorageSize>;
		using CallableType = Detail::IFunctionCallable<ReturnType, Args...>;

		private:
		StorageType _storage;
		CallableType* _callable{ nullptr };

		public:
		Function() = default;
		template <typename Fn>
			requires(FunctionCallableKind<Fn, StorageSize, R, Args...>)
		Function(Fn&& fn) noexcept;
		Function(const ThisType&) noexcept;
		Function(ThisType&&) noexcept;
		~Function() noexcept = default;

		Function& operator=(const ThisType&) noexcept;
		Function& operator=(ThisType&&) noexcept;

		/// @brief Invokes the stored callable object with given \p args.
		/// @param[IN, REQUIRED] args Arguments matching the signature of the stored callable.
		/// @return Returns result of the called function.
		ReturnType operator()(Args... args) const noexcept;

		/// @brief Verifies if the callable object stored is invokable, i.e., not nullptr.
		[[nodiscard]] ORION_FORCE_INLINE Bool8 IsValid() const noexcept;
	};

	// -- Implementation.
	namespace Detail
	{
		template <typename Functor, typename R, typename... Args>
		template <typename Fn>
		FunctionCallable<Functor, R, Args...>::FunctionCallable(Fn&& fn) : fn(Forward<Fn>(fn))
		{
		}

		template <typename Functor, typename R, typename... Args>
		auto FunctionCallable<Functor, R, Args...>::Call(Args... args) noexcept -> ReturnType
		{
			return fn(Forward<Args>(args)...);
		}
	}  // namespace Detail

	template <typename R, typename... Args, USize StorageSize>
	template <typename Fn>
		requires(FunctionCallableKind<Fn, StorageSize, R, Args...>)
	Function<R(Args...), StorageSize>::Function(Fn&& fn) noexcept
	{
		using FunctionCallable = Detail::FunctionCallable<Decay<Fn>, R, Args...>;
		static_assert(sizeof(FunctionCallable) <= StorageSize,
		              "Callable object cannot be stored, because it is too large.");
		Memory::ConstructItem(&_storage, FunctionCallable(Forward<Fn>(fn)));
		_callable = reinterpret_cast<CallableType*>(&_storage.data);
	}

	template <typename R, typename... Args, USize StorageSize>
	Function<R(Args...), StorageSize>::Function(const ThisType& other) noexcept
	{
		Memory::ConstructItem(&_storage, other._storage);
		_callable = reinterpret_cast<CallableType*>(&_storage.data);
	}

	template <typename R, typename... Args, USize StorageSize>
	Function<R(Args...), StorageSize>::Function(ThisType&& other) noexcept
	{
		Memory::ConstructItem(&_storage, other._storage);
		_callable = reinterpret_cast<CallableType*>(&_storage.data);
	}

	template <typename R, typename... Args, USize StorageSize>
	auto Function<R(Args...), StorageSize>::operator=(const ThisType& other) noexcept -> ThisType&
	{
		if (this != &other) {
			if (_callable) {
				Memory::DestructItems(&_storage, 1);
			}
			Memory::ConstructItem(&_storage, other._storage);
			_callable = reinterpret_cast<CallableType*>(&_storage.data);
		}
		return *this;
	}

	template <typename R, typename... Args, USize StorageSize>
	auto Function<R(Args...), StorageSize>::operator=(ThisType&& other) noexcept -> ThisType&
	{
		if (this != &other) {
			if (_callable) {
				Memory::DestructItems(&_storage, 1);
			}
			Memory::ConstructItem(&_storage, Move(other._storage));
			_callable       = reinterpret_cast<CallableType*>(&_storage.data);
			other._callable = nullptr;
		}
		return *this;
	}

	template <typename R, typename... Args, USize StorageSize>
	auto Function<R(Args...), StorageSize>::operator()(Args... args) const noexcept -> ReturnType
	{
		ORION_ASSERT_DEBUG(_callable);
		return _callable->Call(Forward<Args>(args)...);
	}
	template <typename R, typename... Args, USize StorageSize>
	ORION_FORCE_INLINE auto Function<R(Args...), StorageSize>::IsValid() const noexcept -> Bool8
	{
		return !!_callable;
	}
}  // namespace Orion::Engine
