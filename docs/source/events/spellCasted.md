# spellCasted

This event is triggered when any spell is cast successfully; this includes spells cast via scripts. This occurs at the end of the casting animation, just after the magic projectile has been constructed.

```lua
--- @param e spellCastedEventData
local function spellCastedCallback(e)
end
event.register("spellCasted", spellCastedCallback)
```

!!! tip
	This event can be filtered based on the **`source`** event data.

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `target` ([tes3reference](../../types/tes3reference)): The target of the spell. For self-targeted spells, this matches caster.
* `source` ([tes3spell](../../types/tes3spell)): The magic source.
* `expGainSchool` (number): Of all the magic effects in the spell, there is a magic school which the caster has the lowest skill at casting. This school determines which skill will gain experience on a successful cast. This school can be altered, or set to nil to remove experience gain, possibly in favour of your own experience calculation.
* `caster` ([tes3reference](../../types/tes3reference)): The caster of the spell.
* `sourceInstance` ([tes3magicSourceInstance](../../types/tes3magicSourceInstance)): The unique instance of the magic source.
