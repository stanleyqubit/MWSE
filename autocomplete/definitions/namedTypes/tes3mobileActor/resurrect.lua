return {
	type = "method",
	description = [[Resurrects the actor, with more control over the resurrect logic compared to mwscript.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "resetState", type = "boolean", description = "Controls if the stats are reset, the inventory contents are respawned, and the reference recreated. This is the logic that mwscript resurrect uses. It can be useful to reset armor, ammunition, and consumables, if the player has already looted the body. When false, the base stats and inventory are unchanged.", default = true },
			{ name = "moveToStartingLocation", type = "boolean", default = false, description = "Controls if the actor should be moved to its spawn point on resurrection. Requires `resetState` to be true." },
		},
	}},
}