# magicEffectRemoved

This event triggers when a magic effect has been removed from an actor. This includes magic effects that have been resisted or are part of abilities like racial passives and diseases.

```lua
--- @param e magicEffectRemovedEventData
local function magicEffectRemovedCallback(e)
end
event.register(tes3.event.magicEffectRemoved, magicEffectRemovedCallback)
```

!!! tip
	This event can be filtered based on the **`reference`** event data.

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `caster` ([tes3reference](../../types/tes3reference)): *Read-only*. The caster of the magic source that contained the magic effect. Can be `nil`.
* `effect` ([tes3effect](../../types/tes3effect)): *Read-only*. The specific effect that triggered the event. This is equal to `e.source.effects[e.effectIndex]`. Can be `nil`.
* `effectIndex` (number): *Read-only*. The index of the effect in the magic source's effects list.
* `mobile` ([tes3mobileActor](../../types/tes3mobileActor)): *Read-only*. The mobile actor the magic effect is removed from.
* `reference` ([tes3reference](../../types/tes3reference)): *Read-only*. The reference of the mobile actor the magic effect is removed from.
* `source` ([tes3alchemy](../../types/tes3alchemy), [tes3enchantment](../../types/tes3enchantment), [tes3spell](../../types/tes3spell)): *Read-only*. The magic source that contains the effect.
* `sourceInstance` ([tes3magicSourceInstance](../../types/tes3magicSourceInstance)): *Read-only*. The unique instance of the magic source that contains the effect.
* `target` ([tes3reference](../../types/tes3reference)): *Read-only*. The target of the magic source that contained the magic effect. Can be `nil`.

## Examples

!!! example "Example: Show removed effect"

	```lua
	local function onMagicEffectRemoved(e)
		-- Make sure we are only showing messages for effects that are applied to the player.
		if e.mobile == tes3.mobilePlayer then
			-- Get the name of the magic effect that has been removed.
			local effectName = tes3.getMagicEffect(e.effect.id).name
	
			-- Get the name of the spell/enchantment/alchemy this magic effect belongs to.
			local sourceName = e.source.name
	
			-- Show a message containing the effect and spell/enchantment/alchemy names to the player.
			tes3.messageBox("Effect '%s' from '%s' has been removed.", effectName, sourceName)
		end
	end
	event.register(tes3.event.magicEffectRemoved, onMagicEffectRemoved)

	```
