#pragma once

#include "OrionEngine.h"

#include "../Standard/Types/StringView.h"
#include "Core/Standard/Concepts.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/EnumFlag.h"

namespace Orion::Engine
{
	template <typename T>
	concept ConsoleVariableTypeKind = SameAs<T, Bool8> ||  //
	                                  SameAs<T, Int32> ||  //
	                                  SameAs<T, Float32>   //
		;

	/// @brief Represents a variable parameter accessible and/or modifiable from the console by a user.
	struct ConsoleVariable;

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

	/// @brief Represents a command accessible from the console by a user.
	struct ConsoleCommand;

	/// @brief TODO
	template <ConsoleVariableTypeKind T>
	class ConsoleVariableStorage
	{
		public:
		using SizeType = USize;

		static constexpr SizeType k_max_console_variables_of_type = 128;

		private:
		Array<T, k_max_console_variables_of_type> _storage{};
		SizeType _size{ 0UL };

		public:
	};

	/// @brief TODO
	class ConsoleSystem final
	{
		public:
		using SizeType = USize;

		static constexpr SizeType k_max_console_variables_of_type = 128;

		private:
		HashMap<StringView, ConsoleVariable> _console_variables_mapping;
		HashMap<StringView, ConsoleCommand> _console_command_mapping;

		ConsoleVariableStorage<Bool8> _bool8_console_variables{};
		ConsoleVariableStorage<Int32> _int32_console_variables{};
		ConsoleVariableStorage<Float32> _float32_console_variables{};

		public:
		[[nodiscard]] static ConsoleSystem& Get() noexcept;

		/**
		 * @brief Creates new ConsoleVariable of a given type.
		 * @warning \p name parameter **MUST** be unique!
		 */
		template <ConsoleVariableTypeKind T>
		constexpr ConsoleVariable* CreateConsoleVariable(CString name,
		                                                 CString description,
		                                                 T default_value,
		                                                 ConsoleVariableFlags flags = ConsoleVariableFlags::None);

		private:
		ConsoleSystem();

		template <ConsoleVariableTypeKind T>
		[[nodiscard]] ORION_FORCE_INLINE constexpr ConsoleVariableStorage<T>& GetStorage() noexcept;
	};

	// -- Implementation.
	template <ConsoleVariableTypeKind T>
	constexpr auto ConsoleSystem::CreateConsoleVariable(CString name,
	                                                    CString description,
	                                                    T default_value,
	                                                    ConsoleVariableFlags flags) -> ConsoleVariable*
	{
		ORION_ASSERT_DEBUG(name, "Cannot create ConsoleVariable, because name was not provided (nullptr).");
		ORION_ASSERT_DEBUG(*name != '\0', "Cannot create ConsoleVariable, because name is empty (size == 0).");
		ORION_ASSERT_DEBUG(description,
		                   "Cannot create ConsoleVariable, because description was not provided (nullptr)");

		StringView console_variable_name        = ORION_STRINGVIEW(name);
		StringView console_variable_description = ORION_STRINGVIEW(description);

		ORION_ASSERT_DEBUG(!_console_variables_mapping.Contains(console_variable_name),
		                   "Cannot create ConsoleVariable, because it already exists (name == '{}').",
		                   name);

		ConsoleVariableStorage<T>& storage = GetStorage<T>();

		// TODO(SandNoodle): Implement.
		ORION_IGNORE_PARAM(default_value);
		ORION_IGNORE_PARAM(flags);
		ORION_IGNORE_PARAM(console_variable_description);
		ORION_IGNORE_PARAM(storage);

		return nullptr;
	}

	template <ConsoleVariableTypeKind T>
	constexpr ConsoleVariableStorage<T>& ConsoleSystem::GetStorage() noexcept
	{
		ORION_NOT_IMPLEMENTED("ConsoleSystem::GetStorage<T> is not implemented for type.");
	}

	template <>
	constexpr ConsoleVariableStorage<Bool8>& ConsoleSystem::GetStorage() noexcept
	{
		return _bool8_console_variables;
	}

	template <>
	constexpr ConsoleVariableStorage<Int32>& ConsoleSystem::GetStorage() noexcept
	{
		return _int32_console_variables;
	}

	template <>
	constexpr ConsoleVariableStorage<Float32>& ConsoleSystem::GetStorage() noexcept
	{
		return _float32_console_variables;
	}
}  // namespace Orion::Engine
