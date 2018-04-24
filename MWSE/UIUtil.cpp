#include "UIUtil.h"

#define TES3_ui_menu_inventory 0x7D3988

#define TES3_ui_requestMenuModeOff 0x595270
#define TES3_ui_requestMenuModeOn 0x595230
#define TES3_ui_getMenuNode 0x595370

#define TES3_ui_inventory_addTile 0x5CBCC0
#define TES3_ui_inventory_updateIcons 0x5CC910
#define TES3_ui_inventory_equipInventoryTileToPlayer 0x5CE130
#define TES3_ui_data_inventory_updatePaperDoll 0x7B6D04

namespace mwse {
	namespace tes3 {
		namespace ui {
			bool requestMenuModeOff() {
				return reinterpret_cast<signed char(__cdecl *)()>(TES3_ui_requestMenuModeOff)();
			}

			bool requestMenuModeOn(short prop) {
				return reinterpret_cast<signed char(__cdecl *)(short)>(TES3_ui_requestMenuModeOn)(prop);
			}

			TES3::UI::Block* getMenuNode(short id) {
				return reinterpret_cast<TES3::UI::Block*(__cdecl *)(short)>(TES3_ui_getMenuNode)(id);
			}

			short getInventoryMenuId() {
				return *reinterpret_cast<short*>(TES3_ui_menu_inventory);
			}

			TES3::UI::InventoryTile* inventoryAddTile(int something, TES3::UI::InventoryTile* tile) {
				return reinterpret_cast<TES3::UI::InventoryTile*(__cdecl *)(int, TES3::UI::InventoryTile*)>(TES3_ui_inventory_addTile)(something, tile);
			}

			signed char inventoryUpdateIcons() {
				return reinterpret_cast<signed char(__cdecl *)()>(TES3_ui_inventory_updateIcons)();
			}

			signed char equipInventoryTile(TES3::UI::InventoryTile* tile) {
				return reinterpret_cast<signed char(__cdecl *)(TES3::UI::InventoryTile*)>(TES3_ui_inventory_equipInventoryTileToPlayer)(tile);
			}

			void flagPaperDollUpdate() {
				*reinterpret_cast<signed char*>(TES3_ui_data_inventory_updatePaperDoll) = 1;
			}
		}
	}
}
