#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class CalcHitDetectionConeEvent : public ObjectFilteredEvent, public DisableableEvent<CalcHitDetectionConeEvent> {
	public:
		CalcHitDetectionConeEvent(TES3::MobileActor* attacker, float distance, float angleXY, float angleZ);
		sol::table createEventTable();

	protected:
		TES3::MobileActor* m_Attacker;
		float m_Reach;
		float m_AngleXY;
		float m_AngleZ;
	};
}
