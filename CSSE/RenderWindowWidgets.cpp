#include "RenderWindowWidgets.h"

#include "CSDataHandler.h"
#include "CSRecordHandler.h"
#include "CSModelLoader.h"

namespace se::cs::dialog::render_window {
	WidgetsController::WidgetsController() {
		auto loader = DataHandler::get()->recordHandler->modelLoader;

		root = loader->loadNIF("meshes\\mwse\\widgets.nif");
		if (root) {
			auto axisLines = root->getObjectByName("axisLines");
			if (axisLines && axisLines->isOfType(NI::RTTIStaticPtr::NiSwitchNode)) {
				axisMovementWidget = static_cast<NI::SwitchNode*>(axisLines);
				axisMovementWidget->setAppCulled(true);
			}

			auto unitPlane = root->getObjectByName("unitPlane");
			if (unitPlane && unitPlane->isInstanceOfType(NI::RTTIStaticPtr::NiGeometry)) {
				debugUnitPlane = static_cast<NI::Geometry*>(unitPlane);
				debugUnitPlane->setAppCulled(true);
			}

			auto unitSphere = root->getObjectByName("unitSphere");
			if (unitSphere && unitSphere->isInstanceOfType(NI::RTTIStaticPtr::NiGeometry)) {
				debugUnitSphere = static_cast<NI::Geometry*>(unitSphere);
				debugUnitSphere->setAppCulled(true);
			}

			auto unitArrows = root->getObjectByName("unitArrows");
			if (unitArrows && unitArrows->isInstanceOfType(NI::RTTIStaticPtr::NiGeometry)) {
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
			root->parentNode->detachChildHandled(root);
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
