# mouseAxis

The mouseAxis event fires when the mouse is moved, providing deltaX and deltaY values.

```lua
--- @param e mouseAxisEventData
local function mouseAxisCallback(e)
end
event.register("mouseAxis", mouseAxisCallback)
```

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `isSuperDown` (number): True if super (Windows key) is held.
* `deltaY` (number): The difference in vertical mouse position from the previous frame.
* `isControlDown` (number): True if control is held.
* `isAltDown` (number): True if alt  is held.
* `deltaX` (number): The difference in horizontal mouse position from the previous frame.
