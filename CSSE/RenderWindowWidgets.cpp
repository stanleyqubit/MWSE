#include "RenderWindowWidgets.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSModelLoader.h"

namespace se::cs::dialog::render_window {
	WidgetsController::WidgetsController() {
		auto loader = DataHandler::get()->recordHandler->modelLoader;

		root = loader->loadNIF("meshes\\mwse\\widgets.nif");
		if (root) {
			auto axisLines = root->getObjectByNameAndType("axisLines", NI::RTTIStaticPtr::NiSwitchNode);
			if (axisLines) {
				axisMovementWidget = static_cast<NI::SwitchNode*>(axisLines);
				axisMovementWidget->setAppCulled(true);
			}

			auto unitPlane = root->getObjectByNameAndType("unitPlane", NI::RTTIStaticPtr::NiGeometry);
			if (unitPlane) {
				debugUnitPlane = static_cast<NI::Geometry*>(unitPlane);
				debugUnitPlane->setAppCulled(true);
			}

			auto unitSphere = root->getObjectByNameAndType("unitSphere", NI::RTTIStaticPtr::NiGeometry);
			if (unitSphere) {
				debugUnitSphere = static_cast<NI::Geometry*>(unitSphere);
				debugUnitSphere->setAppCulled(true);
			}

			auto unitArrows = root->getObjectByNameAndType("unitArrows", NI::RTTIStaticPtr::NiGeometry);
			if (unitArrows) {
				debugUnitArrows = static_cast<NI::Geometry*>(unitArrows);
				debugUnitArrows->setAppCulled(true);
			}

			root->update();
			root->updateEffects();
			root->updateProperties();
		}
	}

	WidgetsController::~WidgetsController() {
		if (root) {
			root->detachFromParent();
			DataHandler::get()->recordHandler->modelLoader->releaseNIF(root);
		}
	}

	void WidgetsController::show() {
		if (axisMovementWidget) {
			axisMovementWidget->setAppCulled(false);
		}
	}

	void WidgetsController::hide() {
		if (axisMovementWidget) {
			axisMovementWidget->setAppCulled(true);
		}
	}

	bool WidgetsController::isShown() const {
		return axisMovementWidget && !axisMovementWidget->getAppCulled();
	}

	void WidgetsController::setPosition(NI::Vector3& position) {
		if (axisMovementWidget) {
			axisMovementWidget->localTranslate = position;
			axisMovementWidget->update();
		}
	}

	void WidgetsController::setAxis(WidgetsAxis axis) {
		if (axisMovementWidget) {
			axisMovementWidget->setSwitchIndex((int)axis);
		}
	}
}
