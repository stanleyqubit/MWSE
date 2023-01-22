#pragma once

#include "NIGeometry.h"
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
		NI::Pointer<NI::Geometry> debugUnitSphere;
		NI::Pointer<NI::Geometry> debugUnitPlane;
		NI::Pointer<NI::Geometry> debugUnitArrows;

		WidgetsController() {
			auto loader = DataHandler::get()->recordHandler->modelLoader;

			root = loader->loadNIF("meshes\\mwse\\widgets.nif");
			if (!root) {
				root = new NI::Node();
				root->setName("Editor widgetRoot");
				return;
			}

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

		void show() {
			axisMovementWidget->setAppCulled(false);
		}

		void hide() {
			axisMovementWidget->setAppCulled(true);
		}

		bool isShown() const {
			return axisMovementWidget && !axisMovementWidget->getAppCulled();
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