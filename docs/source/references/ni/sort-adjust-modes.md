---
hide:
  - toc
---

# Sort Adjust Modes

!!! tip
	These values are available in Lua by their index in the `ni.sortAdjustMode` table. For example, `ni.sortAdjustMode.off` has a value of `1`.

Index    | Value  | Description
-------- | ------ | ----------------
inherit  | `0`    | Behaves like a standard niNode.
off      | `1`    | Ignores any accumulator attached to the renderer and renders the subtree immediately.
subsort  | `2`    | Replaces the accumulator for the subtree. However, this interferes with the standard alpha accumulator, such that alphas in the subtree are not deferred to draw after opaque objects. 
grouped  | `64`   | If the renderer has an alpha accumulator, the node takes the subtree as a group and defers it render like an alpha. The child nodes are rendered in order, without interior sorting.