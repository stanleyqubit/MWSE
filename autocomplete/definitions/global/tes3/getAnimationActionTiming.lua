return {
	type = "function",
	description = [[This function fetches a dictionary of the timings of the action keys for a specific animation group on an actor. The actor is required, as different actors can use different animations. The result is the a table with action names as keys, and timings as values.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "A reference to the which actor whose animations will be checked." },
			{ name = "group", type = "number", optional = true, description = "The animation group id get the action timings for. Maps to [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) constants." },
		},
	}},
	returns = { name = "result", type = "table" }
}
