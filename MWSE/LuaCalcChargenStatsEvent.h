#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class CalcChargenStatsEvent : public GenericEvent, public DisableableEvent<CalcChargenStatsEvent> {
	public:
		CalcChargenStatsEvent();
	};
}