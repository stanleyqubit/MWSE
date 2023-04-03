#include "LuaCalcHitDetectionConeEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileActor.h"
#include "TES3Reference.h"

namespace mwse::lua::event {
	CalcHitDetectionConeEvent::CalcHitDetectionConeEvent(TES3::MobileActor* attacker, float reach, float angleXY, float angleZ) :
		ObjectFilteredEvent("calcHitDetectionCone", attacker->reference),
		m_Attacker(attacker),
		m_Reach(reach),
		m_AngleXY(angleXY),
		m_AngleZ(angleZ)
	{

	}

	sol::table CalcHitDetectionConeEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["attackerMobile"] = m_Attacker;
		eventData["attacker"] = m_Attacker->reference;
		eventData["reach"] = m_Reach;
		eventData["angleXY"] = m_AngleXY;
		eventData["angleZ"] = m_AngleZ;

		return eventData;
	}

	bool CalcHitDetectionConeEvent::m_EventEnabled = false;
}
