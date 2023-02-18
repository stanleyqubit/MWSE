#include "CSDialogueInfo.h"

#include "CSDataHandler.h"
#include "CSDialogue.h"
#include "CSRecordHandler.h"

namespace se::cs {
	Dialogue* DialogueInfo::getDialogue() const {
		const auto& dialogues = *DataHandler::get()->recordHandler->dialogues;
		for (const auto& dialogue : dialogues) {
			for (const auto& info : dialogue->topics) {
				if (info == this) {
					return dialogue;
				}
			}
		}
		return nullptr;
	}

	bool DialogueInfo::filter(Object* actor, Reference* reference, int source, Dialogue* dialogue) const {
		const auto TES3_DialogueInfo_filter = reinterpret_cast<bool(__thiscall*)(const DialogueInfo*, Object*, Reference*, int, Dialogue*)>(0x402BB7);
		return TES3_DialogueInfo_filter(this, actor, reference, source, dialogue);
	}
}
