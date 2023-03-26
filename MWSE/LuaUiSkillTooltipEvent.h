#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class UiSkillTooltipEvent : public GenericEvent, public DisableableEvent<UiSkillTooltipEvent> {
	public:
		UiSkillTooltipEvent(TES3::UI::Element* tooltip, int skill, int type);
		sol::table createEventTable();

	protected:
		TES3::UI::Element* m_Tooltip;
		int m_Skill;
		int m_Type;
	};
}
