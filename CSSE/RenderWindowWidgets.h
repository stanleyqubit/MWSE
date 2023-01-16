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

		WidgetsController() {
			root = new NI::Node();
			root->setName("Editor widgetRoot");
		}

		void show() {
			root->setAppCulled(false);
		}

		void hide() {
			root->setAppCulled(true);
		}

		auto getAxisLines() {
			auto axis = root->getObjectByName("axisLines");
			if (!axis) {
				auto dataHandler = DataHandler::get();
				if (dataHandler) {
					auto loader = dataHandler->recordHandler->modelLoader;
					axis = loader->loadNIF("meshes\\mwse\\widgets.nif");
					root->attachChild(axis, false);
					root->update();
					root->updateEffects();
					root->updateProperties();
				}
			}
			return reinterpret_cast<NI::SwitchNode*>(axis);
		}

		void setPosition(NI::Vector3& position) {
			auto node = getAxisLines();
			if (node) {
				node->localTranslate = position;
				node->update();
			}
		}

		void setAxis(WidgetsAxis axis) {
			auto node = getAxisLines();
			if (node) {
				node->setSwitchIndex((int)axis);
			}
		}
	};
}