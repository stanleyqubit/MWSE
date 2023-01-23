#pragma once

#include "NIGeometry.h"
#include "NISwitchNode.h"

namespace se::cs::dialog::render_window {
	enum WidgetsAxis {
		X = 0,
		Y = 1,
		Z = 2,
	};

	struct WidgetsController {
		NI::Pointer<NI::Node> root;
		NI::Pointer<NI::SwitchNode> axisMovementWidget;
		NI::Pointer<NI::Geometry> debugUnitSphere;
		NI::Pointer<NI::Geometry> debugUnitPlane;
		NI::Pointer<NI::Geometry> debugUnitArrows;

		WidgetsController();
		~WidgetsController();

		void show();
		void hide();
		bool isShown() const;

		void setPosition(NI::Vector3& position);
		void setAxis(WidgetsAxis axis);
	};
}