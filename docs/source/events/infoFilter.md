# infoFilter
<div class="search_terms" style="display: none">infofilter</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

This event fires when a dialogue info object is filtered. This allows blocking dialogue lines from showing up.

```lua
--- @param e infoFilterEventData
local function infoFilterCallback(e)
end
event.register(tes3.event.infoFilter, infoFilterCallback)
```

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `actor` ([tes3object](../../types/tes3object)): *Read-only*. The actor.
* `dialogue` ([tes3dialogue](../../types/tes3dialogue)): *Read-only*. The dialogue object.
* `info` ([tes3dialogueInfo](../../types/tes3dialogueInfo)): *Read-only*. The dialogue info object.
* `passes` (boolean): Indicates if the filter passes. Set to false to block a line.
* `reference` ([tes3reference](../../types/tes3reference)): *Read-only*. The reference.
* `source` (integer): *Read-only*. The source.

## Examples

!!! example "Example: Blocking greeting lines"

	To block a certain dialogue line efficiently, it's best to check if the line passes first. Only if the line passes do the other checks. Since the game checks each line until it finds one which passes, your callback function registered on this event will be called many times. So, this early out will prevent the game from stalling.

	```lua
	
	--- Don't allow the NPC to speak a greeting line if the player is looking directly at them.
	---@param e infoFilterEventData
	local function blockNPCgreeting(e)
		-- This early check will make sure our function
		-- isn't executing unnecesarily
		if (not e.passes) then
			return
		end
	
		if tes3.menuMode()
		or e.dialogue.id ~= "Hello" then
			-- Make sure to only block the greeting (Hello) lines
			return
		end
	
		if e.reference == tes3.getPlayerTarget() then
			mwse.log("blockNPCGreeting(): Blocking line:\n\"%s\"", e.info.text)
			e.passes = false
		end
	end
	
	event.register(tes3.event.infoFilter, blockNPCgreeting)

	```

