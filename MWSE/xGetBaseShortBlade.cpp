#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3MobileNPC.h"
#include "TES3Skill.h"
#include "TES3Reference.h"

using namespace mwse;

namespace mwse
{
	class xGetBaseShortBlade : mwse::InstructionInterface_t
	{
	public:
		xGetBaseShortBlade();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseShortBlade xGetBaseShortBladeInstance;

	xGetBaseShortBlade::xGetBaseShortBlade() : mwse::InstructionInterface_t(OpCode::xGetBaseShortBlade) {}

	void xGetBaseShortBlade::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xGetBaseShortBlade::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get the associated MACP record.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
#if _DEBUG
			mwse::log::getLog() << "xGetBaseShortBlade: No reference provided." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileNPC();
		if (mobileObject == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetBaseShortBlade: Could not find MACP record for reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the base value of that skill.
		mwse::Stack::getInstance().pushFloat(mobileObject->skills[TES3::SkillID::ShortBlade].base);

		return 0.0f;
	}
}