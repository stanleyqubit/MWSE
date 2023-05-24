---@enum tes3.animationState
return {
	["idle"] = 0,
	["ready"] = 1,
	["swingUp"] = 2,
	["swingDown"] = 3,
	["swingHit"] = 4,
	["swingFollowLight"] = 5,
	["swingFollowMed"] = 6,
	["swingFollowHeavy"] = 7,
	["readyingWeap"] = 8,
	["unreadyWeap"] = 9,
	["casting"] = 10,
	["castingFollow"] = 11,
	---@deprecated
	---Use tes3.animationState.castingFollow instead.
	["casting2"] = 11,
	["readyingMagic"] = 12,
	["unreadyMagic"] = 13,
	["knockdown"] = 14,
	["knockedOut"] = 15,
	["pickingProbing"] = 16,
	["wait"] = 17,
	["dying"] = 18,
	["dead"] = 19,
}
