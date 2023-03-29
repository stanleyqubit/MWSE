#include "NISortAdjustNode.h"

namespace NI {
	SortAdjustNode::SortAdjustNode() : Node() {
		sortingMode = SortAdjustMode::SORTING_INHERIT;
	}

	Pointer<SortAdjustNode> SortAdjustNode::create() {
		return new SortAdjustNode();
	}
}