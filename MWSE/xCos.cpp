#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

using namespace mwse;

namespace mwse
{
	class xCos : mwse::InstructionInterface_t
	{
	public:
		xCos();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xCos xCosInstance;

	xCos::xCos() : mwse::InstructionInterface_t(OpCode::xCos) {}

	void xCos::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xCos::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		mwse::Stack::getInstance().pushFloat(std::cos(mwse::Stack::getInstance().popFloat()));
		return 0.0f;
	}
}