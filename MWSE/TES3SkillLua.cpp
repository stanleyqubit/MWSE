#include "TES3SkillLua.h"

#include "LuaUnifiedHeader.h"
#include "LuaManager.h"

namespace mwse {
	namespace lua {
		void bindTES3Skill() {
			sol::state& state = LuaManager::getInstance().getState();

			state.new_usertype<TES3::Skill>("TES3Skill",
				// Disable construction of this type.
				"new", sol::no_constructor,

				sol::meta_function::to_string, &TES3::Skill::getObjectID,

				//
				// Properties.
				//

				"id", &TES3::Skill::skill,
				"attribute", &TES3::Skill::governingAttribute,
				"specialization", &TES3::Skill::specialization,
				"actions", sol::readonly_property([](TES3::Skill& self) { return std::ref(self.progressActions); })

				);
		}
	}
}
