#pragma once

#include "CSDefines.h"

#include "MemoryUtil.h"

namespace se::cs::window::main {

	constexpr UINT WM_FINISH_INITIALIZATION = 0x414;

	constexpr UINT WM_COMMAND_TOGGLE_LANDSCAPE_EDITING = 0x9CFE;
	constexpr UINT WM_COMMAND_TEST_IN_GAME_MORROWIND = 0xA000;
	constexpr UINT WM_COMMAND_TEST_IN_GAME_OPENMW = 0xA001;

	constexpr UINT MENU_ID_CSSE_SETTINGS = 100000;
	constexpr UINT MENU_ID_CSSE_ABOUT = 100001;

	// Global variables.
	using hInstance = memory::ExternalGlobal<HINSTANCE, 0x6CE930>;
	using ghWnd = memory::ExternalGlobal<HWND, 0x6CE934>;
	using ghToolbar = memory::ExternalGlobal<HWND, 0x6CE970>;

	HWND showObjectEditWindow(BaseObject* object);

	void installPatches();
}