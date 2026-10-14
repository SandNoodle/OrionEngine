#pragma once

#include "OrionEngine.h"

#include "Core/Assert.h"
#include "Core/Log/Logger.h"
#include "Core/Standard/Containers/HashMap.h"
#include "Core/Standard/Containers/String.h"
#include "Core/Standard/Containers/StringView.h"
#include "Core/Standard/EnumFlag.h"
#include "Core/Standard/Memory/Allocators/PlatformAllocator.h"
#include "Core/Standard/Utility/MoveAndForward.h"

namespace Orion::Engine::Console
{
	/// @brief Concept that defines ConsoleVariable types supported by the ConsoleSystem.
	template <typename T>
	concept ConsoleVariableTypeKind = SameAs<T, Bool8>    //
	                               || SameAs<T, Int32>    //
	                               || SameAs<T, Int64>    //
	                               || SameAs<T, UInt32>   //
	                               || SameAs<T, UInt64>   //
	                               || SameAs<T, Float32>  //
	                               || SameAs<T, Float64>  //
	                               || SameAs<T, String>   //
		;

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

		/// ConsoleVariable's value will be saved into a config file and read upon next startup.
		Persistent = 0x10,

		// -- Combined flags.
		/// ConsoleVariable can be both written AND read from console.
		ReadWrite = Read | Write,

		/// ConsoleVariable that can be only used (modified and read) in non-distribution builds of the engine.
		DebugVariable = Debug | Read | Write,
	};
	ORION_ENUM_FLAG(ConsoleVariableFlags)

	/// @brief Base class for all the different kind of objects handled by the ConsoleSystem, such as ConsoleVariable(s)
	/// or ConsoleCommand(s).
	class IConsoleObject
	{
		private:
		String _name;
		String _description;
		String _usage;

		public:
		constexpr IConsoleObject(StringView name, StringView description, StringView usage);

		/// @brief Returns UNIQUE name of this console object.
		[[nodiscard]] ORION_FORCE_INLINE constexpr StringView Name() const noexcept;

		/// @brief Returns description describing purpose of this console object.
		[[nodiscard]] ORION_FORCE_INLINE constexpr StringView Description() const noexcept;

		/// @brief Returns usage hint to the user.
		[[nodiscard]] ORION_FORCE_INLINE constexpr StringView Usage() const noexcept;
	};

	/// @brief Represents a variable parameter accessible and/or modifiable from the console by a user.
	template <ConsoleVariableTypeKind T>
	class ConsoleVariable : public IConsoleObject
	{
		public:
		using BaseType           = IConsoleObject;
		using ThisType           = ConsoleVariable;
		using ValueType          = T;
		using PointerType        = ValueType*;
		using ConstPointerType   = const ValueType*;
		using ReferenceType      = ValueType&;
		using ConstReferenceType = const ValueType&;

		private:
		ValueType _default_value;
		ValueType _current_value;
		ConsoleVariableFlags _flags;

		public:
		ORION_FORCE_INLINE constexpr ConsoleVariable(StringView name,
		                                             ValueType default_value,
		                                             StringView description,
		                                             StringView usage,
		                                             ConsoleVariableFlags flags) noexcept;

		/// @brief Sets value to \p value of this ConsoleVariable.
		/// @param[IN, REQUIRED] value Value to be set to.
		ORION_FORCE_INLINE void SetValue(ValueType value) noexcept;

		/// @brief Returns the current value of this ConsoleVariable.
		ORION_FORCE_INLINE ConstReferenceType Value() const noexcept;

		/// @brief Returns the default value of this ConsoleVariable.
		ORION_FORCE_INLINE ConstReferenceType DefaultValue() const noexcept;

		/// @brief Returns flags associated with this ConsoleVariable.
		ORION_FORCE_INLINE ConsoleVariableFlags Flags() const noexcept;
	};

	/// @brief ConsoleSystem is a system responsible for managing ConsoleVariables and ConsoleCommands.
	class ConsoleSystem final
	{
		public:
		using ThisType = ConsoleSystem;
		using SizeType = USize;

		private:
		HashMap<StringView, IConsoleObject*> _console_objects_mapping;
		Memory::PlatformAllocator _console_objects_allocator;

		public:
		/// @brief Returns singleton for the ConsoleSystem.
		[[nodiscard]] static constexpr ConsoleSystem& Get() noexcept;

		/// @brief Registers new ConsoleVariable of a given type.
		/// @param[IN, REQUIRED] name Name of the ConsoleVariable to be referred by (MUST BE UNIQUE).
		/// @param[IN, REQUIRED] default_value Initial/default value of this ConsoleVariable.
		/// @param[IN, REQUIRED] description Useful description explaining the purpose of a ConsoleVariable.
		/// @param[IN, REQUIRED] usage Helper description describing the intended use.
		/// @param[IN, REQUIRED] flags Usage flags associated with this ConsoleVariable.
		template <ConsoleVariableTypeKind T>
		constexpr ConsoleVariable<T>* RegisterConsoleVariable(StringView name,
		                                                      T default_value,
		                                                      StringView description,
		                                                      StringView usage,
		                                                      ConsoleVariableFlags flags
		                                                      = ConsoleVariableFlags::None) noexcept;

		private:
		constexpr ConsoleSystem() noexcept = default;
	};

	/// @brief Represents a variable parameter accessible and/or modifiable from the console by a user, and which
	/// automatically registers itself into the ConsoleSystem upon creation.
	template <ConsoleVariableTypeKind T>
	class AutoConsoleVariable final
	{
		public:
		using ThisType           = AutoConsoleVariable;
		using StorageType        = ConsoleVariable<T>;
		using ValueType          = StorageType::ValueType;
		using PointerType        = StorageType::PointerType;
		using ConstPointerType   = StorageType::ConstPointerType;
		using ReferenceType      = StorageType::ReferenceType;
		using ConstReferenceType = StorageType::ConstReferenceType;

		private:
		StorageType* _console_variable{ nullptr };

		public:
		ORION_FORCE_INLINE constexpr AutoConsoleVariable(CString name,
		                                                 ValueType&& default_value,
		                                                 CString description,
		                                                 CString usage,
		                                                 ConsoleVariableFlags flags) noexcept;

		/// @brief Sets value to \p value of this AutoConsoleVariable.
		/// @param[IN, REQUIRED] value Value to be set to.
		ORION_FORCE_INLINE void SetValue(ValueType value) noexcept;

		/// @brief Returns the current value of this AutoConsoleVariable.
		ORION_FORCE_INLINE ConstReferenceType Value() const noexcept;

		/// @brief Returns the default value of this AutoConsoleVariable.
		ORION_FORCE_INLINE ConstReferenceType DefaultValue() const noexcept;

		/// @brief Returns flags associated with this AutoConsoleVariable.
		ORION_FORCE_INLINE ConsoleVariableFlags Flags() const noexcept;
	};

	// -- Implementation.
	constexpr IConsoleObject::IConsoleObject(StringView name, StringView description, StringView usage)
		: _name(name.Data(), name.Size()),
		  _description(description.Data(), description.Size()),
		  _usage(usage.Data(), usage.Size())
	{
	}

	ORION_FORCE_INLINE constexpr StringView IConsoleObject::Name() const noexcept
	{
		return StringView(_name.Data(), _name.Size());
	}

	ORION_FORCE_INLINE constexpr StringView IConsoleObject::Description() const noexcept
	{
		return StringView(_description.Data(), _description.Size());
	}

	ORION_FORCE_INLINE constexpr StringView IConsoleObject::Usage() const noexcept
	{
		return StringView(_usage.Data(), _usage.Size());
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE constexpr ConsoleVariable<T>::ConsoleVariable(StringView name,
	                                                                 ValueType default_value,
	                                                                 StringView description,
	                                                                 StringView usage,
	                                                                 ConsoleVariableFlags flags) noexcept
		: BaseType(name, description, usage),
		  _default_value(default_value),
		  _current_value(default_value),
		  _flags(flags)
	{
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto ConsoleVariable<T>::SetValue(ValueType value) noexcept -> void
	{
		_current_value = value;
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto ConsoleVariable<T>::Value() const noexcept -> ConstReferenceType
	{
		return _current_value;
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto ConsoleVariable<T>::DefaultValue() const noexcept -> ConstReferenceType
	{
		return _default_value;
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto ConsoleVariable<T>::Flags() const noexcept -> ConsoleVariableFlags
	{
		return _flags;
	}

	constexpr auto ConsoleSystem::Get() noexcept -> ThisType&
	{
		static ConsoleSystem console_system;
		return console_system;
	}

	template <ConsoleVariableTypeKind T>
	constexpr auto ConsoleSystem::RegisterConsoleVariable(StringView name,
	                                                      T default_value,
	                                                      StringView description,
	                                                      StringView usage,
	                                                      ConsoleVariableFlags flags) noexcept -> ConsoleVariable<T>*
	{
		ORION_ASSERT_DEBUG(!name.IsEmpty(),
		                   "Cannot create ConsoleVariable, because name was not provided (size == 0).");
		ORION_ASSERT_DEBUG(!description.IsEmpty(),
		                   "Cannot create ConsoleVariable, because description was not provided (size == 0).");
		ORION_ASSERT_DEBUG(!usage.IsEmpty(),
		                   "Cannot create ConsoleVariable, because usage was not provided (size == 0).");

		StringView console_variable_name = StringView(name);
		if (_console_objects_mapping.Contains(console_variable_name)) {
			ORION_LOG_WARN("Cannot create ConsoleVariable, because it already exists (name == '{}').", name);
			return nullptr;
		}

		ConsoleVariable<T>* variable = Memory::AllocateConstruct<ConsoleVariable<T>>(
			_console_objects_allocator, name, description, usage, Move(default_value), flags);
		if (!variable) {
			ORION_LOG_WARN("Cannot create ConsoleVariable, because its allocation failed (nullptr).");
			return nullptr;
		}

		_console_objects_mapping.Insert(console_variable_name, variable);
		return variable;
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE constexpr AutoConsoleVariable<T>::AutoConsoleVariable(CString name,
	                                                                         ValueType&& default_value,
	                                                                         CString description,
	                                                                         CString usage,
	                                                                         ConsoleVariableFlags flags) noexcept
		: _console_variable(ConsoleSystem::Get().RegisterConsoleVariable(StringView(name),
	                                                                     StringView(description),
	                                                                     StringView(usage),
	                                                                     default_value,
	                                                                     flags))
	{
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto AutoConsoleVariable<T>::SetValue(ValueType value) noexcept -> void
	{
		ORION_ASSERT_DEBUG_SLOW(_console_variable);
		return _console_variable->SetValue(Move(value));
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto AutoConsoleVariable<T>::Value() const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(_console_variable);
		return _console_variable->Value();
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto AutoConsoleVariable<T>::DefaultValue() const noexcept -> ConstReferenceType
	{
		ORION_ASSERT_DEBUG_SLOW(_console_variable);
		return _console_variable->DefaultValue();
	}

	template <ConsoleVariableTypeKind T>
	ORION_FORCE_INLINE auto AutoConsoleVariable<T>::Flags() const noexcept -> ConsoleVariableFlags
	{
		ORION_ASSERT_DEBUG_SLOW(_console_variable);
		return _console_variable->Flags();
	}
}  // namespace Orion::Engine::Console
