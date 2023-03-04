---
hide:
  - toc
---

# Lighting Modes

!!! tip
	These values are available in Lua by their index in the `mge.lightingMode` table. For example, `mge.lightingMode.perPixel` has a value of `1`.

Index                  | Value  | Description
---------------------- | ------ | ------------------------
vertex                 | `0`    | Use standard per-vertex lighting, as in vanilla engine.
perPixel               | `1`    | Use per-pixel lighting shader.
perPixelInteriorOnly   | `2`    | Use per-pixel lighting shader, but only in interior cells.
