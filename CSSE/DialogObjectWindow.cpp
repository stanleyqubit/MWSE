#include "DialogObjectWindow.h"

#include "LogUtil.h"
#include "MemoryUtil.h"
#include "StringUtil.h"
#include "WindowsUtil.h"

#include "NIIteratedList.h"

#include "CSBaseObject.h"

namespace se::cs::dialog::object_window {
	using namespace memory;
	using namespace windows;

	static HWND objectWindowSearchControl = NULL;

	//
	// Patch: Optimize displaying of objects dialog tabs.
	//

	const auto NI_IteratedList_Begin = reinterpret_cast<NI::IteratedList<BaseObject*>::Node * (__thiscall*)(NI::IteratedList<BaseObject*>*)>(0x401E29);
	NI::IteratedList<BaseObject*>::Node* __fastcall PatchSpeedUpObjectWindow_PauseRedraws(NI::IteratedList<BaseObject*>* list) {
		auto result = NI_IteratedList_Begin(list);

		if (result) {
			const auto listView = se::memory::MemAccess<HWND>::Get(0x6CEFD0);
			SendMessageA(listView, WM_SETREDRAW, FALSE, NULL);
		}

		return result;
	}

	const auto NI_IteratedList_Next = reinterpret_cast<NI::IteratedList<BaseObject*>::Node * (__thiscall*)(NI::IteratedList<BaseObject*>*)>(0x403D8C);
	NI::IteratedList<BaseObject*>::Node* __fastcall PatchSpeedUpObjectWindow_ResumeRedraws(NI::IteratedList<BaseObject*>* list) {
		auto result = NI_IteratedList_Next(list);

		if (result == nullptr) {
			const auto listView = se::memory::MemAccess<HWND>::Get(0x6CEFD0);
			SendMessageA(listView, WM_SETREDRAW, TRUE, NULL);
		}

		return result;
	}

	//
	// Patch: Allow filtering of object window.
	//

	static std::string currentSearchText;

	const auto CS_AddObjectToWindow = reinterpret_cast<bool(__stdcall*)(LPARAM, BaseObject*)>(0x43C260);
	bool CALLBACK PatchFilterObjectWindow(LPARAM a1, BaseObject* object) {
		if (currentSearchText.empty()) {
			return CS_AddObjectToWindow(a1, object);
		}

		std::string objectId = object->getObjectID();
		string::to_lower(objectId);

		if (objectId.find(currentSearchText) != std::string::npos) {
			return CS_AddObjectToWindow(a1, object);
		}

		return false;
	}

	//
	// Patch: Extend Object Window message handling.
	//

	void CALLBACK PatchDialogProc_BeforeSize(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Update view menu.
		auto mainWindow = GetMenu(se::memory::MemAccess<HWND>::Get(0x6CE934));
		if (wParam) {
			if (wParam == SIZE_MINIMIZED) {
				CheckMenuItem(mainWindow, 40199u, MF_BYCOMMAND);
			}
		}
		else {
			CheckMenuItem(mainWindow, 40199u, MF_CHECKED);
		}

		auto tabControl = GetDlgItem(hDlg, 1042);
		auto objectListView = GetDlgItem(hDlg, 1041);
		auto searchLabel = GetDlgItem(hDlg, CONTROL_ID_FILTER_LABEL);
		auto searchEdit = GetDlgItem(hDlg, CONTROL_ID_FILTER_EDIT);

		// Update globals.
		se::memory::MemAccess<HWND>::Set(0x6CF08C, tabControl);
		se::memory::MemAccess<HWND>::Set(0x6CEFD0, objectListView);
		objectWindowSearchControl = searchEdit;

		const auto mainWidth = LOWORD(lParam);
		const auto mainHeight = HIWORD(lParam);

		// Make room for our new search bar.
		MoveWindow(tabControl, 0, 0, mainWidth, mainHeight, TRUE);

		// Update list view area.
		RECT tabContentRect = {};
		SetRect(&tabContentRect, 0, 0, mainWidth, mainHeight - 30);
		SendMessageA(tabControl, TCM_ADJUSTRECT, 0, (LPARAM)&tabContentRect);
		MoveWindow(objectListView, tabContentRect.left, tabContentRect.top, tabContentRect.right - tabContentRect.left, tabContentRect.bottom - tabContentRect.top, TRUE);

		// Update the search bar placement.
		SetWindowPos(searchLabel, NULL, tabContentRect.right - 500 - 58, tabContentRect.bottom + 7, 54, 22, 0);
		SetWindowPos(objectWindowSearchControl, NULL, tabContentRect.right - 500, tabContentRect.bottom + 4, 500, 24, SWP_DRAWFRAME);
	}

	void CALLBACK PatchDialogProc_AfterCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto filterEdit = GetDlgItem(hWnd, CONTROL_ID_FILTER_EDIT);
		if (filterEdit) {
			objectWindowSearchControl = filterEdit;
			return;
		}

		auto hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

		CreateWindowExA(WS_EX_RIGHT, "STATIC", "Filter:", WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_NOPREFIX, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_FILTER_LABEL, hInstance, NULL);

		objectWindowSearchControl = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)CONTROL_ID_FILTER_EDIT, hInstance, NULL);
		if (objectWindowSearchControl == NULL) {
			log::stream << "ERROR: Could not create search control!" << std::endl;
			return;
		}

		Edit_LimitText(objectWindowSearchControl, 31);
	}

	inline void OnNotifyFromMainListView(HWND hWnd, UINT msg, WPARAM id, LPARAM lParam) {
		const auto hdr = (NMHDR*)lParam;

		if (hdr->code == LVN_KEYDOWN) {
			const auto keydownHDR = (LV_KEYDOWN*)hdr;
			if (keydownHDR->wVKey == 'F' && isKeyDown(VK_CONTROL)) {
				SetFocus(objectWindowSearchControl);
			}
		}
	}

	inline void OnNotifyFromMainTabControl(HWND hWnd, UINT msg, WPARAM id, LPARAM lParam) {
		const auto hdr = (NMHDR*)lParam;

		if (hdr->code == TCN_SELCHANGING) {
			SetWindowTextA(objectWindowSearchControl, "");
		}
	}

	inline void PatchDialogProc_BeforeNotify(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (wParam) {
		case CONTROL_ID_LIST_VIEW:
			OnNotifyFromMainListView(hWnd, msg, wParam, lParam);
			break;
		case CONTROL_ID_TABS:
			OnNotifyFromMainTabControl(hWnd, msg, wParam, lParam);
			break;
		}
	}

	inline void OnFilterEditChanged(HWND hWnd) {
		// Get current search text. The buffer is fine as 32 because we set a character limit of 31.
		char buffer[32] = {};
		auto length = GetWindowTextA(objectWindowSearchControl, buffer, 32);

		// Transform the search text to lowercase and clear stray characters.
		std::string newText(buffer, length);
		string::to_lower(newText);

		if (!string::equal(currentSearchText, newText)) {
			currentSearchText = std::move(newText);

			// Fire a refresh function. But disable drawing throughout so we don't get ugly flashes.
			const auto listView = se::memory::MemAccess<HWND>::Get(0x6CEFD0);
			SendMessageA(listView, WM_SETREDRAW, FALSE, NULL);
			SendMessageA(hWnd, 1043, 0, 0);
			SendMessageA(listView, WM_SETREDRAW, TRUE, NULL);
			RedrawWindow(listView, NULL, NULL, RDW_INVALIDATE);
		}
	}

	inline void PatchDialogProc_BeforeCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		const auto command = HIWORD(wParam);
		const auto id = LOWORD(wParam);
		switch (command) {
		case EN_CHANGE:
			switch (id) {
			case CONTROL_ID_FILTER_EDIT:
				OnFilterEditChanged(hWnd);
				break;
			}
			break;
		}
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		bool blockNormalExecution = false;

		// Handle pre-patches.
		switch (msg) {
		case WM_DESTROY:
			objectWindowSearchControl = NULL;
			break;
		case WM_SIZE:
			PatchDialogProc_BeforeSize(hWnd, msg, wParam, lParam);
			return FALSE;
		case WM_NOTIFY:
			PatchDialogProc_BeforeNotify(hWnd, msg, wParam, lParam);
			break;
		case WM_COMMAND:
			PatchDialogProc_BeforeCommand(hWnd, msg, wParam, lParam);
			break;
		}

		if (blockNormalExecution) {
			return TRUE;
		}

		// Call original function.
		const auto CS_ObjectWindowDialogProc = reinterpret_cast<WNDPROC>(0x451320);
		auto result = CS_ObjectWindowDialogProc(hWnd, msg, wParam, lParam);

		// Handle post-patches.
		switch (msg) {
		case WM_INITDIALOG:
			PatchDialogProc_AfterCreate(hWnd, msg, wParam, lParam);
			break;
		}

		return result;
	}
	
	//
	// Main patching function.
	//

	void installPatches() {
		// Patch: Optimize displaying of objects dialog tabs.
		genCallEnforced(0x43C1B4, 0x401E29, reinterpret_cast<DWORD>(PatchSpeedUpObjectWindow_PauseRedraws));
		genCallEnforced(0x43C1CC, 0x403D8C, reinterpret_cast<DWORD>(PatchSpeedUpObjectWindow_ResumeRedraws));

		// Patch: Extend Object Window message handling.
		genJumpEnforced(0x402FD1, 0x451320, reinterpret_cast<DWORD>(PatchDialogProc));

		// Patch: Allow filtering of object window.
		genJumpEnforced(0x401F0F, 0x43C260, reinterpret_cast<DWORD>(PatchFilterObjectWindow));
	}
}