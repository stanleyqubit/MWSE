# mouseWheel

The mouseWheel event fires when the mouse wheel is used, providing a delta value.

```lua
--- @param e mouseWheelEventData
local function mouseWheelCallback(e)
end
event.register("mouseWheel", mouseWheelCallback)
```

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `isSuperDown` (number): True if super (Windows key) is held.
* `isAltDown` (number): True if alt is held.
* `isControlDown` (number): True if control is held.
* `delta` (number): The direction and strength of the mouse wheel movement. This value is hardware dependent.
