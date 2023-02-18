#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "TES3Defines.h"

namespace mwse::lua::event {
	class EnchantChargeUseEvent : public GenericEvent, public DisableableEvent<EnchantChargeUseEvent> {
	public:
		EnchantChargeUseEvent(TES3::Enchantment* enchant, TES3::MobileActor* caster, TES3::MagicSourceInstance* magicSource, float chargeRequired);
		sol::table createEventTable();
		sol::object getEventOptions();

	protected:
		TES3::Enchantment* m_Enchant;
		TES3::MobileActor* m_Caster;
		TES3::MagicSourceInstance* m_MagicSource;
		float m_ChargeRequired;
	};
}
