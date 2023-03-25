#include "NITriBasedGeometry.h"

namespace NI {
	const auto NI_TriBasedGeometry_ctorFromData = reinterpret_cast<void(__thiscall*)(TriBasedGeometry*, TriBasedGeometryData*)>(0x6EFEA0);
	TriBasedGeometry::TriBasedGeometry(TriBasedGeometryData* data) {
		NI_TriBasedGeometry_ctorFromData(this, data);
	}
}
