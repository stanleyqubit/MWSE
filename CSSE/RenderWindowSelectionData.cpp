#include "RenderWindowSelectionData.h"

namespace se::cs::dialog::render_window {
	void SelectionData::recalculateCenter() {
		const auto SelectionData_recalculateCenter = reinterpret_cast<void(__thiscall*)(SelectionData*)>(0x402919);
		SelectionData_recalculateCenter(this);
	}

	void SelectionData::recalculateRadius() {
		const auto SelectionData_recalculateRadius = reinterpret_cast<void(__thiscall*)(SelectionData*)>(0x403DF0);
		SelectionData_recalculateRadius(this);
	}

	void SelectionData::recalculateBound() {
		recalculateCenter();
		recalculateRadius();
	}

	void SelectionData::clear(bool unknownFlag) {
		const auto SelectioNData_clear = reinterpret_cast<void(__thiscall*)(SelectionData*, bool)>(0x403391);
		SelectioNData_clear(this, unknownFlag);
	}
}