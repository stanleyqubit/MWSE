# tes3inventoryTile
<div class="search_terms" style="display: none">tes3inventorytile, inventorytile</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

An inventory tile, connecting an item, its data, and various related UI elements.

## Properties

### `count`
<div class="search_terms" style="display: none">count</div>

*Read-only*. The number of items that the tile represents.

**Returns**:

* `result` (number)

***

### `element`
<div class="search_terms" style="display: none">element</div>

*Read-only*. The UI element associated with the tile.

**Returns**:

* `result` ([tes3uiElement](../../types/tes3uiElement))

***

### `flags`
<div class="search_terms" style="display: none">flags</div>

*Read-only*. Direct access to the tile's flags. Use `.isBartered`, `.isBoundItem`, or `.isEquipped` to access individual flags.

**Returns**:

* `result` (number)

***

### `isBartered`
<div class="search_terms" style="display: none">isbartered, bartered</div>

If `true`, the tile represents an item being bartered.

**Returns**:

* `result` (boolean)

***

### `isBoundItem`
<div class="search_terms" style="display: none">isbounditem, bounditem</div>

*Read-only*. If `true`, the tile represents a bound item.

**Returns**:

* `result` (boolean)

***

### `isEquipped`
<div class="search_terms" style="display: none">isequipped, equipped</div>

If `true`, the tile represents an item that is equipped.

**Returns**:

* `result` (boolean)

***

### `item`
<div class="search_terms" style="display: none">item</div>

*Read-only*. The item the tile represents.

**Returns**:

* `result` ([tes3item](../../types/tes3item))

***

### `itemData`
<div class="search_terms" style="display: none">itemdata</div>

*Read-only*. The item data associated with this tile.

**Returns**:

* `result` ([tes3itemData](../../types/tes3itemData))

***

### `type`
<div class="search_terms" style="display: none">type</div>

*Read-only*. The type of tile represented. Maps to values in [`tes3.inventoryTileType`](https://mwse.github.io/MWSE/references/inventory-tile-types/) namespace.

**Returns**:

* `result` (number)

