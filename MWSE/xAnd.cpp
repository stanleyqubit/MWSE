#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

using namespace mwse;

namespace mwse
{
	class xAnd : mwse::InstructionInterface_t
	{
	public:
		xAnd();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xAnd xAndInstance;

	xAnd::xAnd() : mwse::InstructionInterface_t(OpCode::xAnd) {}

	void xAnd::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xAnd::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		long param1 = mwse::Stack::getInstance().popLong();
		long param2 = mwse::Stack::getInstance().popLong();

		mwse::Stack::getInstance().pushLong(param1 && param2);

		return 0.0f;
	}
}