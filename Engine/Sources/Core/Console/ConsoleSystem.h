#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/EnumFlag.h"
#include "Core/Standard/Limits.h"

namespace Orion::Engine::Console
{
	namespace Detail
	{
		/// @brief TODO
		template <typename T>
		struct ConsoleVariableValue
		{
			T default_value;
			T current_value;
		};

		/// @brief TODO
		template <typename T, typename S = UInt16, S Capacity = 128>
		class ConsoleVariableStorage : Array<ConsoleVariableValue<T>, Capacity>
		{
			public:
			using ThisType           = ConsoleVariableStorage;
			using BaseType           = Array<ConsoleVariableValue<T>, Capacity>;
			using ValueType          = T;
			using SizeType           = S;
			using ReferenceType      = ValueType&;
			using ConstReferenceType = const ValueType&;
			using PointerType        = ValueType*;
			using ConstPointerType   = const ValueType*;

			static constexpr SizeType k_maximum_console_variables = Capacity;
			static constexpr SizeType k_invalid_index             = NumericLimits<SizeType>::Max();

			private:
			SizeType _current_size{ 0UL };

			public:
			[[nodiscard]] constexpr SizeType Add(ConstReferenceType default_value) noexcept
			{
				if (_current_size + 1 >= k_maximum_console_variables) {
					return k_invalid_index;
				}
				BaseType::operator[](_current_size).default_value = default_value;
				BaseType::operator[](_current_size).current_value = default_value;
				return _current_size++;
			}
		};
	}  // namespace Detail

	/// @brief Underlying types of console variables supported by the ConsoleSystem.
#define ORION_CONSOLE_SYSTEM_VARIABLE_TYPE_LIST \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(Bool8)   \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(Int32)   \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(Int64)   \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(UInt32)  \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(UInt64)  \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(Float32) \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(Float64) \
	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(String)

	/// @brief Represents properties of a given ConsoleVariable.
	/// Any and all modifications via code are still possible no matter the flags set.
	enum class ConsoleVariableFlags : UInt8
	{
		/// ConsoleVariable that cannot be accessed by users, i.e. has no flags set.
		None = 0x0,

		/// ConsoleVariable can be only accessed in non-distribution builds of the engine.
		Debug = 0x1,

		/// ConsoleVariable can be only read by from console, but not written.
		Read = 0x2,

		/// ConsoleVariable can be written to from console, but not read.
		Write = 0x4,

		/// ConsoleVariable can be modified only by a server, not a client.
		Server = 0x8,

		// -- Combined flags.
		/// ConsoleVariable can be both written AND read from console.
		ReadWrite = Read | Write,

		/// ConsoleVariable that can be only used (modified and read) in non-distribution builds of the engine.
		DebugVariable = Debug | Read | Write,
	};
	ORION_ENUM_FLAG(ConsoleVariableFlags)

	/// @brief Represents a variable parameter accessible and/or modifiable from the console by a user.
	struct ConsoleVariable
	{
		String name;
		String description;
		String usage;
		UInt16 storage_index;
		ConsoleVariableFlags flags;
	};

	/// @brief TODO
	class ConsoleSystem final
	{
		public:
		using SizeType = USize;

		private:
		HashMap<StringView, ConsoleVariable> _console_variables_mapping;

#define ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(type) Detail::ConsoleVariableStorage<type> _##type##_console_variables;
		ORION_CONSOLE_SYSTEM_VARIABLE_TYPE_LIST
#undef ORION_CONSOLE_SYSTEM_VARIABLE_TYPE

		public:
		/// @brief TODO
		[[nodiscard]] static ConsoleSystem& Get() noexcept;

		/**
		 * @brief Registers new ConsoleVariable of a given type.
		 *
		 * @param name Name of the ConsoleVariable to be referred by (MUST BE UNIQUE).
		 * @param description TODO
		 * @param usage TODO
		 * @param default_value TODO
		 * @param flags TODO
		 */
		template <typename T>
		constexpr void CreateConsoleVariable(CString name,
		                                     CString description,
		                                     CString usage,
		                                     T&& default_value,
		                                     ConsoleVariableFlags flags = ConsoleVariableFlags::None);

		private:
		ConsoleSystem();

		template <typename T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr Detail::ConsoleVariableStorage<T>& GetStorage() noexcept;
	};

	// -- Implementation.
	template <typename T>
	constexpr auto ConsoleSystem::CreateConsoleVariable(CString name,
	                                                    CString description,
	                                                    CString usage,
	                                                    T&& default_value,
	                                                    ConsoleVariableFlags flags) -> void
	{
		ORION_ASSERT_DEBUG(name, "Cannot create ConsoleVariable, because name was not provided (nullptr).");
		ORION_ASSERT_DEBUG(*name != '\0', "Cannot create ConsoleVariable, because name is empty (size == 0).");
		ORION_ASSERT_DEBUG(description,
		                   "Cannot create ConsoleVariable, because description was not provided (nullptr)");

		StringView console_variable_name = ORION_STRINGVIEW(name);
		if (_console_variables_mapping.Contains(console_variable_name)) {
			ORION_LOG_WARN("Cannot create ConsoleVariable, because it already exists (name == '{}').", name);
			return;
		}

		using StorageType                            = Detail::ConsoleVariableStorage<T>;
		StorageType& storage                         = GetStorage<T>();
		typename StorageType::SizeType storage_index = storage.Add(Move(default_value));
		if (storage_index >= StorageType::k_maximum_console_variables) {
			ORION_LOG_WARN("Cannot create ConsoleVariable, because limit for this type was reached.");
			return;
		}

		ConsoleVariable console_variable{
			.name          = ORION_STRING(name),
			.description   = ORION_STRING(description),
			.usage         = ORION_STRING(usage),
			.storage_index = storage_index,
			.flags         = flags,
		};
		_console_variables_mapping.Insert(console_variable_name, Move(console_variable));
	}

	template <typename T>
	ORION_FORCE_INLINE constexpr Detail::ConsoleVariableStorage<T>& ConsoleSystem::GetStorage() noexcept
	{
		ORION_NOT_IMPLEMENTED("ConsoleSystem::GetStorage<T> is not implemented for type.");
	}

#define ORION_CONSOLE_SYSTEM_VARIABLE_TYPE(type)                                                            \
	template <>                                                                                             \
	ORION_FORCE_INLINE constexpr Detail::ConsoleVariableStorage<type>& ConsoleSystem::GetStorage() noexcept \
	{                                                                                                       \
		return _##type##_console_variables;                                                                 \
	}

	ORION_CONSOLE_SYSTEM_VARIABLE_TYPE_LIST
#undef ORION_CONSOLE_SYSTEM_VARIABLE_TYPE

#undef ORION_CONSOLE_SYSTEM_VARIABLE_TYPE_LIST
}  // namespace Orion::Engine::Console
