#include "DialogEditNPCObjectWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"

namespace se::cs::dialog::edit_npc_object_window {

	//
	// Configuration
	//

	constexpr auto ENABLE_ALL_OPTIMIZATIONS = true;
	constexpr auto LOG_PERFORMANCE_RESULTS = false;

	//
	// Extended window messages.
	//

	std::optional<LRESULT> messageResult;

	std::chrono::high_resolution_clock::time_point initializationTimer;

	void PatchDialogProc_BeforeInitialize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if constexpr (LOG_PERFORMANCE_RESULTS) {
			initializationTimer = std::chrono::high_resolution_clock::now();
		}

		// Optimize redraws.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			SendDlgItemMessageA(hWnd, CONTROL_ID_BLOOD_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CLASS_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_DETAILS_LIST, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_FACTION_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_FACTION_RANK_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_HAIR_LIST, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_HEAD_LIST, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_RACE_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, FALSE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SKILLS_LIST, WM_SETREDRAW, FALSE, NULL);
		}
	}

	void PatchDialogProc_AfterInitialize(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Restore redraws.
		if constexpr (ENABLE_ALL_OPTIMIZATIONS) {
			SendDlgItemMessageA(hWnd, CONTROL_ID_BLOOD_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_CLASS_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_DETAILS_LIST, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_FACTION_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_FACTION_RANK_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_HAIR_LIST, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_HEAD_LIST, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_RACE_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SCRIPT_COMBO, WM_SETREDRAW, TRUE, NULL);
			SendDlgItemMessageA(hWnd, CONTROL_ID_SKILLS_LIST, WM_SETREDRAW, TRUE, NULL);

		}

		if constexpr (LOG_PERFORMANCE_RESULTS) {
			auto timeToInitialize = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - initializationTimer);
			log::stream << "Displaying NPC object data took " << timeToInitialize.count() << "ms" << std::endl;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		messageResult = {};

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_BeforeInitialize(hWnd, msg, wParam, lParam);
			break;
		}

		if (messageResult) {
			return messageResult.value();
		}

		// Call original function.
		const auto CS_VanillaDialogProc = reinterpret_cast<WNDPROC>(0x423060);
		const auto vanillaResult = CS_VanillaDialogProc(hWnd, msg, wParam, lParam);

		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitialize(hWnd, msg, wParam, lParam);
			break;
		}

		return messageResult.value_or(vanillaResult);
	}

	//
	//
	//

	void installPatches() {
		using memory::genJumpEnforced;

		// Patch: Extend handling.
		genJumpEnforced(0x40313E, 0x423060, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
