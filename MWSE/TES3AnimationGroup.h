#pragma once

#include "TES3Object.h"

namespace TES3 {
	struct AnimationGroup : BaseObject {
		struct SoundGenNote {
			int startIndex; // 0x0
			float timeIndex; // 0x4
			unsigned char volume; // 0x8
			float pitch; // 0xC
			Sound* sound; // 0x10

			SoundGenNote() = delete;
			~SoundGenNote() = delete;
		};
		unsigned char groupId; // 0x10
		unsigned int actionCount; // 0x14
		int* actionFrameNumbers; // 0x18
		float* actionTimes; // 0x1C
		AnimationGroup* nextGroup; // 0x20
		unsigned int soundGenCount; // 0x24
		SoundGenNote* soundGenNotes; // 0x28

		AnimationGroup() = delete;
		~AnimationGroup() = delete;
	};
	static_assert(sizeof(AnimationGroup) == 0x2C, "TES3::AnimationGroup failed size validation");
	static_assert(sizeof(AnimationGroup::SoundGenNote) == 0x14, "TES3::AnimationGroup::SoundGenNote failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::AnimationGroup)
