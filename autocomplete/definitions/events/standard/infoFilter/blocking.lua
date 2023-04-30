
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
