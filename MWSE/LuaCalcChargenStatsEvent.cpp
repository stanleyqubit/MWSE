#include "LuaCalcChargenStatsEvent.h"
#include "LuaManager.h"

namespace mwse::lua::event {
	CalcChargenStatsEvent::CalcChargenStatsEvent() : GenericEvent("calcChargenStats") { }
	bool CalcChargenStatsEvent::m_EventEnabled = true;
}
