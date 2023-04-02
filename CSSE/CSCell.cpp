#include "CSCell.h"

namespace se::cs {
	bool Cell::getIsInterior() const {
		return cellFlags & CellFlag::Interior;
	}
}
