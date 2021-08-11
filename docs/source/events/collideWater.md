# collideWater

This event is triggered when a mobile object collides with water.

```lua
--- @param e collideWaterEventData
local function collideWaterCallback(e)
end
event.register("collideWater", collideWaterCallback)
```

!!! tip
	This event can be filtered based on the **`reference`** event data.

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `reference` ([tes3mobileObject](../../types/tes3mobileObject)): `mobile`'s reference.
* `mobile` ([tes3mobileObject](../../types/tes3mobileObject)): Mobile object which collided.
* `inWater` (boolean): 
