#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Enchantment.h"

using namespace mwse;

namespace mwse
{
	class xSetEnchantInfo : mwse::InstructionInterface_t
	{
	public:
		xSetEnchantInfo();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xSetEnchantInfo xSetEnchantInfoInstance;

	xSetEnchantInfo::xSetEnchantInfo() : mwse::InstructionInterface_t(OpCode::xSetEnchantInfo) {}

	void xSetEnchantInfo::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xSetEnchantInfo::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameters.
		mwseString& enchantId = virtualMachine.getString(Stack::getInstance().popLong());
		long type = Stack::getInstance().popLong();
		long cost = Stack::getInstance().popLong();
		long charge = Stack::getInstance().popLong();
		bool autocalc = (Stack::getInstance().popLong() != 0);

		// Validate type.
		if (type < 0 || type > 3) {
#if _DEBUG
			mwse::log::getLog() << "xSetEnchantInfo: Type out of range." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return false;
		}

		// Validate autocalc.
		if (autocalc < 0 || autocalc > 1) {
#if _DEBUG
			mwse::log::getLog() << "xSetEnchantInfo: Autocalc value of range." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return false;
		}

		TES3::Enchantment* enchant = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Enchantment>(enchantId, TES3::ObjectType::Enchantment);
		if (enchant == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetEnchantInfo: No effect found given id '" << enchantId << "'." << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return false;
		}

		// Set values.
		enchant->castType = static_cast<TES3::EnchantmentCastType>(type);
		enchant->chargeCost = cost;
		enchant->maxCharge = charge;
		enchant->vTable.object->setAutoCalc(enchant, autocalc);

		mwse::Stack::getInstance().pushLong(true);
		return 0.0f;
	}
}