#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Creature.h"
#include "TES3Reference.h"

using namespace mwse;

namespace mwse
{
	class xGetGold : mwse::InstructionInterface_t
	{
	public:
		xGetGold();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xGetGold xGetGoldInstance;

	xGetGold::xGetGold() : mwse::InstructionInterface_t(OpCode::xGetGold) {}

	void xGetGold::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xGetGold::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetGold: Called on invalid reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(0);
			return 0.0f;
		}

		// Try to get the gold from the macp record.
		short gold = 0;
		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject) {
			gold = mobileObject->barterGold;
		}
		else {
			// Get the gold based on the base record type if we can't get it from macp.
			TES3::BaseObject* baseRecord = reference->baseObject;
			if (baseRecord->objectType == TES3::ObjectType::NPC) {
				TES3::NPCInstance* npc = reinterpret_cast<TES3::NPCInstance*>(baseRecord);
				if (npc->baseNPC) {
					gold = npc->baseNPC->barterGold;
				}
				else {
#if _DEBUG
					mwse::log::getLog() << "xGetGold: Could not get base NPC record for \"" << npc->objectID << "\"" << std::endl;
#endif
				}
			}
			else if (baseRecord->objectType == TES3::ObjectType::Creature) {
				TES3::CreatureInstance* creature = reinterpret_cast<TES3::CreatureInstance*>(baseRecord);
				if (creature->baseCreature) {
					gold = creature->baseCreature->barterGold;
				}
				else {
#if _DEBUG
					mwse::log::getLog() << "xGetGold: Could not get base creature record for \"" << creature->objectID << "\"" << std::endl;
#endif
				}
			}
		}

		// Push the base value of that skill.
		mwse::Stack::getInstance().pushShort(gold);

		return 0.0f;
	}
}