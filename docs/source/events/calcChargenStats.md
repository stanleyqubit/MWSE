# calcChargenStats
<div class="search_terms" style="display: none">calcchargenstats</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

The calcChargenStats event triggers when the stats review menu is shown.
	
	The player stats will have already been calculated, but on this event you may modify them before the menu is displayed.

```lua
--- @param e calcChargenStatsEventData
local function calcChargenStatsCallback(e)
end
event.register(tes3.event.calcChargenStats, calcChargenStatsCallback)
```

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Examples

!!! example "Example: In this example we apply some basic skill bonuses and maluses based on the player gender."

	```lua
	
	local skillBonusesF = {
		[tes3.skill.alchemy] = 15,
		[tes3.skill.illusion] = 25,
		[tes3.skill.mysticism] = -5,
	}
	
	local skillBonusesM = {
		[tes3.skill.athletics] = 10,
		[tes3.skill.unarmored] = 10,
		[tes3.skill.spear] = 15,
	}
	
	event.register(tes3.event.calcChargenStats, function()
	    local skills = tes3.player.baseObject.female and skillBonusesF or skillBonusesM
	    for k, v in pairs(skills) do
		    tes3.modStatistic{reference = tes3.player, skill = k, value = v}
	    end
	end)

	```

