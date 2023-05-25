#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	// Enter frame event.
	class SimulatedEvent : public GenericEvent, public DisableableEvent<SimulatedEvent> {
	public:
		SimulatedEvent();
		sol::table createEventTable();
	};
}
