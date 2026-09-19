#include "Core/Console/ConsoleSystem.h"

namespace Orion::Engine::Console
{
	ConsoleObject::ConsoleObject(StringView name, StringView description, StringView usage)
		: _name(name.Data(), name.Size()),
		  _description(description.Data(), description.Size()),
		  _usage(usage.Data(), usage.Size())
	{
	}

	ORION_FORCE_INLINE StringView ConsoleObject::Name() const noexcept
	{
		return StringView(_name.Data(), _name.Size());
	}

	ORION_FORCE_INLINE StringView ConsoleObject::Description() const noexcept
	{
		return StringView(_description.Data(), _description.Size());
	}

	ORION_FORCE_INLINE StringView ConsoleObject::Usage() const noexcept
	{
		return StringView(_usage.Data(), _usage.Size());
	}
}  // namespace Orion::Engine::Console
