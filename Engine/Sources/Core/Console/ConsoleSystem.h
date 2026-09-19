#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/Containers/Vector.h"
#include "Core/Standard/EnumFlag.h"
#include "Core/Standard/Limits.h"
#include "Core/Standard/Memory/Allocators/PlatformAllocator.h"

namespace Orion::Engine::Console
{
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

	enum class ConsoleCommandFlags : UInt8
	{
		None = 0x0,
	};
	ORION_ENUM_FLAG(ConsoleCommandFlags);

	/// @brief TODO
	class ConsoleObject
	{
		private:
		String _name;
		String _description;
		String _usage;

		public:
		ConsoleObject(StringView name, StringView description, StringView usage);

		[[nodiscard]] ORION_FORCE_INLINE StringView Name() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE StringView Description() const noexcept;
		[[nodiscard]] ORION_FORCE_INLINE StringView Usage() const noexcept;
	};

	/// @brief Represents a variable parameter accessible and/or modifiable from the console by a user.
	template <typename T>
	class ConsoleVariable final : public ConsoleObject
	{
		public:
		using BaseType  = ConsoleObject;
		using ThisType  = ConsoleVariable;
		using ValueType = T;

		private:
		ValueType _default_value;
		ValueType _current_value;
		ConsoleVariableFlags _flags;

		public:
		ConsoleVariable(StringView name,
		                StringView description,
		                StringView usage,
		                ValueType default_value,
		                ConsoleVariableFlags flags) noexcept
			: BaseType(name, description, usage),
			  _default_value(default_value),
			  _current_value(default_value),
			  _flags(flags)
		{
		}
	};

	/// @brief TODO
	using ConsoleCommandCallbackFn = void (*)();

	/// @brief TODO
	class ConsoleCommand final : public ConsoleObject
	{
		public:
		using BaseType = ConsoleObject;

		private:
		ConsoleCommandCallbackFn _callback;
		ConsoleCommandFlags _flags;

		public:
		ConsoleCommand(StringView name,
		               StringView description,
		               StringView usage,
		               ConsoleCommandCallbackFn callback_function,
		               ConsoleCommandFlags flags) noexcept
			: BaseType(name, description, usage), _callback(callback_function), _flags(flags)
		{
		}
	};

	/// @brief TODO
	template <typename Allocator = Memory::PlatformAllocator>
	class ConsoleSystem final
	{
		public:
		using ThisType      = ConsoleSystem;
		using AllocatorType = Allocator;
		using SizeType      = USize;

		private:
		HashMap<StringView, ConsoleObject*> _console_objects_mapping;
		AllocatorType _console_objects_allocator;

		public:
		/// @brief TODO
		[[nodiscard]] static ConsoleSystem& Get() noexcept;

		/// @brief Registers new ConsoleVariable of a given type.
		/// @param name Name of the ConsoleVariable to be referred by (MUST BE UNIQUE).
		/// @param description TODO
		/// @param usage TODO
		/// @param default_value TODO
		/// @param flags TODO
		template <typename T>
		constexpr void RegisterConsoleVariable(CString name,
		                                       CString description,
		                                       CString usage,
		                                       T&& default_value,
		                                       ConsoleVariableFlags flags = ConsoleVariableFlags::None) noexcept;

		private:
		constexpr ConsoleSystem() noexcept;
	};

	// -- Implementation.
	template <typename Allocator>
	auto ConsoleSystem<Allocator>::Get() noexcept -> ThisType&
	{
		static ConsoleSystem console_system;
		return console_system;
	}

	template <typename Allocator>
	template <typename T>
	constexpr auto ConsoleSystem<Allocator>::RegisterConsoleVariable(CString name,
	                                                                 CString description,
	                                                                 CString usage,
	                                                                 T&& default_value,
	                                                                 ConsoleVariableFlags flags) noexcept -> void
	{
		ORION_ASSERT_DEBUG(name, "Cannot create ConsoleVariable, because name was not provided (nullptr).");
		ORION_ASSERT_DEBUG(*name != '\0', "Cannot create ConsoleVariable, because name is empty (size == 0).");
		ORION_ASSERT_DEBUG(description,
		                   "Cannot create ConsoleVariable, because description was not provided (nullptr).");
		ORION_ASSERT_DEBUG(*description != '\0',
		                   "Cannot create ConsoleVariable, because description is empty (size == 0).");
		ORION_ASSERT_DEBUG(usage, "Cannot create ConsoleVariable, because usage was not provided (nullptr).");
		ORION_ASSERT_DEBUG(*usage != '\0', "Cannot create ConsoleVariable, because usage is empty (size == 0).");

		StringView console_variable_name = StringView(name);
		if (_console_objects_mapping.Contains(console_variable_name)) {
			ORION_LOG_WARN("Cannot create ConsoleVariable, because it already exists (name == '{}').", name);
			return;
		}

		ConsoleVariable<T>* variable = Memory::AllocateConstruct<ConsoleVariable<T>>(_console_objects_allocator,
		                                                                             StringView(name),
		                                                                             StringView(description),
		                                                                             StringView(usage),
		                                                                             default_value,
		                                                                             flags);
		_console_objects_mapping.Insert(console_variable_name, variable);
	}

	template <typename Allocator>
	constexpr ConsoleSystem<Allocator>::ConsoleSystem() noexcept
	{
	}
}  // namespace Orion::Engine::Console
