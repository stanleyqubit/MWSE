
---@enum tes3.keyTransition
return {
	["isDown"] = 1,
	["downThisFrame"] = 2,
	["upThisFrame"] = 3,
	["changedThisFrame"] = 4,

	-- Legacy values.
	["test"] = 0,
	---@deprecated
	["up"] = 1,
	---@deprecated
	["down"] = 2,
	---@deprecated
	["toggled"] = 3,
}
