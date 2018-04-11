/************************************************************************
               VirtualMachine.h - Copyright (c) 2008 The MWSE Project
                https://github.com/MWSE/MWSE/

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#pragma once

#include "TES3Script.h"
#include "TES3Reference.h"
#include "mwAdapter.h"
#include "VMHookInterface.h"
#include "VMExecuteInterface.h"
#include "mwseString.h"

using namespace mwse;

namespace mwse
{
	class VirtualMachine : public VMHookInterface, VMExecuteInterface
	{
	public:
		VirtualMachine();
		virtual float executeOperation(OpCode::OpCode_t opcode, HookContext &context, TES3::Script* script);
		virtual void loadParametersForOperation(OpCode::OpCode_t opcode, HookContext &context, TES3::Script* script);
		virtual bool isOpcode(const OpCode::OpCode_t opcode);
		virtual long* getScriptIP();

		virtual TES3::Reference * getReference();
		virtual TES3::Reference * getReference(const char *id);
		virtual void setReference(TES3::Reference * reference);
		virtual TES3::Reference * getCurrentTarget();
		virtual TES3::BaseObject * getTemplate(const char *id);

		//local variables, methods to access local variables
		virtual long getLongVariable(int index);							//by index
		virtual long getLongVariable(const char *id);						//by name
		virtual long getLongVariable(int index, TES3::Reference& reference);	//foreign
		virtual void setLongVariable(int index, long value);
		virtual void setLongVariable(const char *id, long value);
		virtual void setLongVariable(int index, long value , TES3::Reference &reference);

		virtual short getShortVariable(int index);
		virtual short getShortVariable(const char *id);
		virtual short getShortVariable(int index, TES3::Reference& reference);
		virtual void setShortVariable(int index, short value);
		virtual void setShortVariable(const char *id, short value);
		virtual void setShortVariable(int index, short value , TES3::Reference &reference);

		virtual float getFloatVariable(int index);
		virtual float getFloatVariable(const char *id);
		virtual float getFloatVariable(int index, TES3::Reference& reference);
		virtual void setFloatVariable(int index, float value);
		virtual void setFloatVariable(const char *id, float value);
		virtual void setFloatVariable(int index, float value , TES3::Reference& reference);

		//global variables, methods to access global variables
		virtual long getLongGlobal(const char *id);
		virtual void setLongGlobal(const char *id, long value);
		
		virtual short getShortGlobal(const char *id);
		virtual void setShortGlobal(const char *id, short value);

		virtual float getFloatGlobal(const char *id);
		virtual void setFloatGlobal(const char *id, float value);

		//getParameters
		virtual char getByteValue(bool peek = false);
		virtual short getShortValue(bool peek = false);
		virtual long getLongValue(bool peek =false);
		virtual float getFloatValue(bool peek = false);

		virtual mwseString& getString(long fromStack);	//only ment for stack-based syntax!, parameter-based syntax functions should use getStringParameter!!!


		// Debug method to print information about the current script.
		virtual void dumpScriptVariables();

		HookContext context;

		TES3::Script* getScript();

	private:
		// The currently executing script.
		TES3::Script * script;
		void setScript(TES3::Script* script);
		
		// Current context (registers, etc).
		HookContext getHookContext();
		void setHookContext(HookContext context);

		long *mwScriptIP;

		// Last executed script.
		TES3::Script * oldscript;

		// Called when the currently executed script changes.
		void OnScriptChange();
	};
};