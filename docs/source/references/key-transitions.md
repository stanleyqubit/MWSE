---
hide:
  - toc
---

# Key Transitions

!!! tip
	These values are available in Lua by their index in the `tes3.keyTransition` table. For example, `tes3.keyTransition.isDown` has a value of `1`.

Index            | Value | Description
---------------- | ----- | --------------------------------------------
isDown           | `1`   | Tests if the key is currently pressed down.
downThisFrame    | `2`   | Tests if the key was pressed this frame.
upThisFrame      | `3`   | Tests if the key was released this frame.
changedThisFrame | `4`   | Tests if the key changed state this frame.
