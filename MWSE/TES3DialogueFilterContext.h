#pragma once

#include "TES3DialogueConditional.h"

namespace TES3 {
	struct DialogueFilterContext {
		struct ConditionalContext {
			DialogueFilterContext* parentContext = nullptr;
			DialogueConditional* conditional = nullptr;
			std::optional<bool> resultOverride;
			DialogueConditionalComparator compareOperator = DialogueConditionalComparator::Equal;
			std::optional<float> compareValue;

			void load();

			void flipCompareOperator();
		};
		Object* speaker = nullptr;
		Actor* speakerBaseActor = nullptr;
		Reference* speakerReference = nullptr;
		MobileActor* speakerMobile = nullptr;
		int source = 0;
		Dialogue* dialogue = nullptr;
		const DialogueInfo* dialogueInfo = nullptr;

		Actor* filterActor = nullptr;
		Race* filterRace = nullptr;
		Class* filterClass = nullptr;
		Faction* filterFaction = nullptr;
		Cell* filterCell = nullptr;
		Faction* filterPlayerFaction = nullptr;
		const char* filterResultScript = nullptr;
		ConditionalContext conditionalContexts[6];

		DialogueFilterContext(Object* speaker, Reference* reference, int source, Dialogue* dialogue, const DialogueInfo* info);
		void setConditional(size_t index, DialogueConditional* conditional);
	};
}
