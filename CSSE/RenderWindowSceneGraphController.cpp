#include "RenderWindowSceneGraphController.h"

namespace se::cs::dialog::render_window {
	bool __cdecl SceneGraphController::initialize(SceneGraphController* controller) {
		// Zero out the structure again to handle newly added fields.
		ZeroMemory(controller, sizeof(SceneGraphController));

		// Call existing function.
		const auto SceneGraphController_initialize = reinterpret_cast<bool(__cdecl*)(SceneGraphController*)>(0x449930);
		if (!SceneGraphController_initialize(controller)) {
			return false;
		}

		return true;
	}
}