#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "mwAdapter.h"
#include "VirtualMachine.h"
#include "ScriptUtil.h"

using namespace mwse;

namespace mwse
{
	class xHasItemEquipped : mwse::InstructionInterface_t
	{
	public:
		xHasItemEquipped();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xHasItemEquipped xHasItemEquippedInstance;

	xHasItemEquipped::xHasItemEquipped() : mwse::InstructionInterface_t(OpCode::xHasItemEquipped) {}

	void xHasItemEquipped::loadParameters(mwse::VMExecuteInterface &virtualMachine) {
	}

	float xHasItemEquipped::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameters.
		mwseString& id = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get script reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xHasItemEquipped: Called on invalid reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get item template.
		TES3::BaseObject* itemTemplate = virtualMachine.getTemplate(id.c_str());
		if (itemTemplate == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xHasItemEquipped: No template found with id '" << id << "'." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Call the original function.
		TES3::Script* script = virtualMachine.getScript();
		bool result = mwse::mwscript::HasItemEquipped(script, reference, itemTemplate);

		mwse::Stack::getInstance().pushLong(result);

		return 0.0f;
	}
}