return {
	type = "event",
	description = [[The calcChargenStats event triggers when the stats review menu is shown.
	
	The player stats will have already been calculated, but on this event you may modify them before the menu is displayed.]],
	examples = {
		["genderStats"] = {
			title = "In this example we apply some basic skill bonuses and maluses based on the player gender.",
		}
	}
}