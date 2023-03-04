#include "DialogCellWindow.h"

#include "MemoryUtil.h"
#include "LogUtil.h"

namespace se::cs::dialog::preview_window {
	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Handle pre-patches.
		switch (msg) {

		}

		// Call original function.
		const auto CS_CellViewDialogProc = reinterpret_cast<WNDPROC>(0x455AA0);
		auto result = CS_CellViewDialogProc(hWnd, msg, wParam, lParam);

		// Handle post-patches.
		switch (msg) {

		}

		return result;
	}

	void installPatches() {
		using memory::genJumpEnforced;
		using memory::writeBytesUnprotected;

		// Patch: Don't reset viewing angle when a new object is chosen.
		const BYTE patchSkipSetTransform[] = { 0xEB, 0x25 };
		writeBytesUnprotected(0x45752D, patchSkipSetTransform, 2);

		// Patch: Extend window messages.
		genJumpEnforced(0x403189, 0x455AA0, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
