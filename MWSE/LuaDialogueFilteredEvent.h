#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Dialogue.h"

namespace mwse::lua::event {
	class DialogueFilteredEvent : public ObjectFilteredEvent, public DisableableEvent<DialogueFilteredEvent> {
	public:
		DialogueFilteredEvent(TES3::Dialogue* dialogue, TES3::DialogueInfo* info, TES3::Object* actor, TES3::Reference* reference, TES3::Dialogue::GetFilteredInfoContext context);
		sol::table createEventTable();

	protected:
		TES3::Dialogue* m_Dialogue;
		TES3::DialogueInfo* m_DialogueInfo;
		TES3::Object* m_Actor;
		TES3::Reference* m_Reference;
		TES3::Dialogue::GetFilteredInfoContext m_Context;
	};
}
