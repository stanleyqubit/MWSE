--- Returns true if the actor is currently speaking a voiceover line.
---@param mobile tes3mobileActor
---@return boolean isSpeaking
local function isSpeaking(mobile)
	local ad = mobile.animationController.animationData
	return (ad.lipsyncLevel ~= -1)
end
