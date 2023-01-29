#pragma once

#include "CSDefines.h"

#include "NIBound.h"

#include "MemoryUtil.h"

namespace se::cs::dialog::render_window {
	struct SelectionData {
		enum class RotationAxis : unsigned int {
			X = 1,
			Z = 2,
			Y = 3,
		};
		struct Target {
			Reference* reference; // 0x0
			Target* previous; // 0x4
			Target* next; // 0x8
		};
		Target* firstTarget; // 0x0
		unsigned int numberOfTargets; // 0x4
		NI::Bound bound; // 0x8

		void recalculateCenter();
		void recalculateRadius();
		void recalculateBound();

		void clear(bool unknownFlag = true);

		static inline auto get() {
			return memory::ExternalGlobal<SelectionData*, 0x6CE968>::get();
		}

		Target* getLastTarget() const;
	};
}