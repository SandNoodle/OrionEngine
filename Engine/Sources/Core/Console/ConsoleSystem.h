#pragma once

#include "OrionEngine.h"

#include "Core/Standard/Concepts.h"
#include "Core/Standard/Containers/Array.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Strings/StringView.h"

namespace Orion::Engine
{
	template <typename T>
	concept ConsoleVariableTypeKind = SameAs<T, Bool8> ||    //
	                                  SameAs<T, Int32> ||    //
	                                  SameAs<T, Int64> ||    //
	                                  SameAs<T, Float32> ||  //
	                                  SameAs<T, Float64> ||  //
	                                  SameAs<T, StringView>  //
		;

	/// @brief Represents a variable parameter accessible and/or modifiable from the console by a user.
	struct ConsoleVariable;

	/// @brief Represents properties of a given ConsoleVariable.
	/// Any and all modifications via code are still possible no matter the flags set.
	enum ConsoleVariableFlags : UInt8
	{
		/// ConsoleVariable that cannot be accessed by users, i.e. has no flags set.
		ConsoleVariableFlag_None = 0x0,

		/// ConsoleVariable can be only accessed in non-distribution builds of the engine.
		ConsoleVariableFlag_Debug = 0x1,

		/// ConsoleVariable can be only read by from console, but not written.
		ConsoleVariableFlag_Read = 0x2,

		/// ConsoleVariable can be written to from console, but not read.
		ConsoleVariableFlag_Write = 0x4,

		/// ConsoleVariable can be modified only by a server, not a client.
		ConsoleVariableFlag_Server = 0x8,

		// -- Combined flags.
		/// ConsoleVariable can be both written AND read from console.
		ConsoleVariableFlag_ReadWrite = ConsoleVariableFlag_Read | ConsoleVariableFlag_Write,

		/// ConsoleVariable that can be only used (modified and read) in non-distribution builds of the engine.
		ConsoleVariableFlag_DebugVariable = ConsoleVariableFlag_Debug  //
		                                  | ConsoleVariableFlag_Read   //
		                                  | ConsoleVariableFlag_Write,
	};

	/// @brief Represents a command accessible from the console by a user.
	struct ConsoleCommand;

	template <typename T>
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

	/// @brief Represents a
	class ConsoleSystem final
	{
		public:
		using SizeType = USize;

		static constexpr SizeType k_max_console_variables_of_type = 128;

		private:
		HashMap<StringView::HashType, ConsoleVariable> _console_variables_mapping;
		HashMap<StringView::HashType, ConsoleCommand> _console_command_mapping;

		ConsoleVariableStorage<Bool8> _bool8_console_variables{};
		ConsoleVariableStorage<Int64> _int64_console_variables{};
		ConsoleVariableStorage<Float64> _float64_console_variables{};

		public:
		[[nodiscard]] static ConsoleSystem& Get() noexcept;

		/**
		 * @brief Creates new ConsoleVariable of a given type.
		 * @warning \p name parameter **MUST** be unique!
		 */
		template <ConsoleVariableTypeKind T>
		[[nodiscard]] constexpr ConsoleVariable* CreateConsoleVariable(const char* name,
		                                                               const char* description,
		                                                               T default_value,
		                                                               ConsoleVariableFlags flags
		                                                               = ConsoleVariableFlag_None);

		private:
		ConsoleSystem();
	};

	// -- Implementation.
	template <ConsoleVariableTypeKind T>
	constexpr auto ConsoleSystem::CreateConsoleVariable(const char* name,
	                                                    const char* description,
	                                                    T default_value,
	                                                    ConsoleVariableFlags flags) -> ConsoleVariable*
	{
		ORION_ASSERT_DEBUG(name, "Cannot create ConsoleVariable, because name was not provided (nullptr).");
		ORION_ASSERT_DEBUG(*name != '\0', "Cannot create ConsoleVariable, becuase name is empty (size == 0).");
		ORION_ASSERT_DEBUG(description,
		                   "Canont create ConsoleVariable, becuase description was not provided (nullptr)");

		StringView console_variable_name        = StringView::FromStringLiteral(name);
		StringView console_variable_description = StringView::FromStringLiteral(description);

		ORION_ASSERT_DEBUG(!_console_variables_mapping.Contains(console_variable_name.Hash()),
		                   "Cannot create ConsoleVariable, because it already exists (name == '{}').",
		                   name);

		return nullptr;
	}
}  // namespace Orion::Engine
