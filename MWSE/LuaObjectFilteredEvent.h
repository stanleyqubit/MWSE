#pragma once

#include "LuaGenericEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	// 
	class ObjectFilteredEvent : public GenericEvent {
	public:
		ObjectFilteredEvent(const char* name, const TES3::BaseObject* filter);
		sol::object getEventOptions();

	protected:
		const TES3::BaseObject* m_EventFilter;
	};
}
