#include "DialogLandscapeEditSettingsWindow.h"

#include "CSLandTexture.h"

#include "WinUIUtil.h"

namespace se::cs::dialog::landscape_edit_settings_window {

	namespace LandscapeEditFlag {
		enum LandscapeEditFlag : unsigned int {
			ShowEditRadius = 0x1,
			FlattenVertices = 0x2,
			EditColors = 0x4,
			SoftenVertices = 0x8,
		};
	}

	using gLandscapeEditFlags = memory::ExternalGlobal<unsigned int, 0x6CE9C8>;

	bool getLandscapeEditFlag(LandscapeEditFlag::LandscapeEditFlag flag) {
		return gLandscapeEditFlags::get() & flag;
	}

	void setLandscapeEditFlag(LandscapeEditFlag::LandscapeEditFlag flag, bool set) {
		auto& flags = gLandscapeEditFlags::get();
		if (set) {
			flags |= flag;
		}
		else {
			flags &= ~flag;
		}
	}

	bool getEditLandscapeColor() {
		return getLandscapeEditFlag(LandscapeEditFlag::EditColors);
	}

	void setEditLandscapeColor(bool set) {
		auto hWnd = gWindowHandle::get();

		setLandscapeEditFlag(LandscapeEditFlag::EditColors, set);
		CheckDlgButton(hWnd, CONTROL_ID_EDIT_COLORS_CHECKBOX, set ? BST_CHECKED : BST_UNCHECKED);

		if (set) {
			setFlattenLandscapeVertices(false);
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX), FALSE);
			setSoftenLandscapeVertices(false);
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX), FALSE);
		}
		else {
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX), TRUE);
			EnableWindow(GetDlgItem(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX), TRUE);
		}
	}

	bool getFlattenLandscapeVertices() {
		return getLandscapeEditFlag(LandscapeEditFlag::FlattenVertices);
	}

	void setFlattenLandscapeVertices(bool set) {
		auto hWnd = gWindowHandle::get();

		setLandscapeEditFlag(LandscapeEditFlag::FlattenVertices, set);
		CheckDlgButton(hWnd, CONTROL_ID_FLATTEN_VERTICES_CHECKBOX, set ? BST_CHECKED : BST_UNCHECKED);

		if (set) {
			setSoftenLandscapeVertices(false);
			setEditLandscapeColor(false);
		}
	}

	bool getSoftenLandscapeVertices() {
		return getLandscapeEditFlag(LandscapeEditFlag::SoftenVertices);
	}

	void setSoftenLandscapeVertices(bool set) {
		auto hWnd = gWindowHandle::get();

		setLandscapeEditFlag(LandscapeEditFlag::SoftenVertices, set);
		CheckDlgButton(hWnd, CONTROL_ID_SOFTEN_VERTICES_CHECKBOX, set ? BST_CHECKED : BST_UNCHECKED);

		if (set) {
			setFlattenLandscapeVertices(false);
			setEditLandscapeColor(false);
		}
	}

	LandTexture* getSelectedTexture() {
		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return nullptr;
		}

		auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);
		auto selected = ListView_GetNextItem(textureList, -1, LVNI_SELECTED);
		if (selected == -1) {
			return nullptr;
		}

		LVITEMA queryData = {};
		queryData.mask = LVIF_PARAM;
		queryData.iItem = selected;
		if (!ListView_GetItem(textureList, &queryData)) {
			return nullptr;
		}

		return reinterpret_cast<LandTexture*>(queryData.lParam);
	}

	bool setSelectTexture(LandTexture* landTexture) {
		return setSelectTexture(landTexture->texture);
	}

	bool setSelectTexture(NI::Texture* texture) {
		if (texture == nullptr) {
			return false;
		}

		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return false;
		}

		LVITEMA queryData = {};
		queryData.mask = LVIF_PARAM;
		auto textureList = GetDlgItem(hWnd, CONTROL_ID_TEXTURE_LIST);
		auto textureCount = ListView_GetItemCount(textureList);
		for (auto row = 0; row < textureCount; ++row) {
			queryData.iItem = row;
			if (!ListView_GetItem(textureList, &queryData)) {
				continue;
			}

			auto landTexture = reinterpret_cast<LandTexture*>(queryData.lParam);
			if (!landTexture) {
				continue;
			}

			if (landTexture->texture == texture) {
				ListView_SetItemState(textureList, row, LVIS_SELECTED, LVIS_SELECTED);
				ListView_EnsureVisible(textureList, row, TRUE);
				return true;
			}
		}

		return false;
	}


	bool incrementEditRadius() {
		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return false;
		}

		auto radius = winui::GetDlgItemSignedInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT).value_or(1);
		radius = std::min(radius + 1, 30);
		SetDlgItemInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT, radius, FALSE);

		return true;
	}

	bool decrementEditRadius() {
		auto hWnd = gWindowHandle::get();
		if (hWnd == NULL) {
			return false;
		}

		auto radius = winui::GetDlgItemSignedInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT).value_or(1);
		radius = std::max(radius - 1, 1);
		SetDlgItemInt(hWnd, CONTROL_ID_EDIT_RADIUS_EDIT, radius, FALSE);

		return true;
	}
}
