#pragma once

#include "NISwitchNode.h"
#include "CSModelLoader.h"

namespace se::cs::dialog::render_window {

	enum WidgetsAxis {
		X = 0,
		Y = 1,
		Z = 2,
	};

	struct WidgetsController {
		NI::Pointer<NI::Node> root;
		NI::Pointer<NI::SwitchNode> axisMovementWidget;

		WidgetsController() {
			root = new NI::Node();
			root->setName("Editor widgetRoot");

			auto loader = DataHandler::get()->recordHandler->modelLoader;
			auto maybeValidAxisMovementWidget = loader->loadNIF("meshes\\mwse\\widgets.nif");
			if (maybeValidAxisMovementWidget && maybeValidAxisMovementWidget->isOfType(NI::RTTIStaticPtr::NiSwitchNode)) {
				axisMovementWidget = static_cast<NI::SwitchNode*>(maybeValidAxisMovementWidget);
				root->attachChild(axisMovementWidget, false);
				root->update();
				root->updateEffects();
				root->updateProperties();
			}
		}

		void show() {
			root->setAppCulled(false);
			root->update();
		}

		void hide() {
			root->setAppCulled(true);
			root->update();
		}

		void setPosition(NI::Vector3& position) {
			if (axisMovementWidget) {
				axisMovementWidget->localTranslate = position;
				axisMovementWidget->update();
			}
		}

		void setAxis(WidgetsAxis axis) {
			if (axisMovementWidget) {
				axisMovementWidget->setSwitchIndex((int)axis);
			}
		}
	};
}