#pragma once

#include "MemoryUtil.h"

#include "CSDefines.h"
#include "NIDefines.h"

namespace se::cs::dialog::landscape_edit_settings_window {

	constexpr UINT DIALOG_ID = 203;

	constexpr UINT CONTROL_ID_EDIT_COLORS_CHECKBOX = 1008;
	constexpr UINT CONTROL_ID_EDIT_FALLOFF_EDIT = 1002;
	constexpr UINT CONTROL_ID_EDIT_RADIUS_EDIT = 1000;
	constexpr UINT CONTROL_ID_FLATTEN_VERTICES_CHECKBOX = 1006;
	constexpr UINT CONTROL_ID_SHOW_EDIT_RADIUS_CHECKBOX = 1007;
	constexpr UINT CONTROL_ID_SOFTEN_VERTICES_CHECKBOX = 1009;
	constexpr UINT CONTROL_ID_TEXTURE_LIST = 1492;

	using gWindowHandle = memory::ExternalGlobal<HWND, 0x6CE95C>;

	bool getEditLandscapeColor();
	void setEditLandscapeColor(bool value);
	bool getFlattenLandscapeVertices();
	void setFlattenLandscapeVertices(bool value);
	bool getSoftenLandscapeVertices();
	void setSoftenLandscapeVertices(bool value);

	LandTexture* getSelectedTexture();
	bool setSelectTexture(LandTexture* landTexture);
	bool setSelectTexture(NI::Texture* texture);

	bool incrementEditRadius();
	bool decrementEditRadius();
}
