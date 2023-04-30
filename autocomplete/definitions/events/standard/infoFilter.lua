return {
	type = "event",
	description = "This event fires when a dialogue info object is filtered. This allows blocking dialogue lines from showing up.",
	related = { "infoGetText", "infoLinkResolve", "infoResponse", "infoFilter", "postInfoResponse" },
	eventData = {
		["info"] = {
			type = "tes3dialogueInfo",
			readOnly = true,
			description = "The dialogue info object.",
		},
		["actor"] = {
			type = "tes3object",
			readOnly = true,
			description = "The actor.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference.",
		},
		["source"] = {
			type = "integer",
			readOnly = true,
			description = "The source.",
		},
		["dialogue"] = {
			type = "tes3dialogue",
			readOnly = true,
			description = "The dialogue object.",
		},
		["passes"] = {
			type = "boolean",
			description = "Indicates if the filter passes. Set to false to block a line.",
		},
	},
	examples = {
		["blocking"] = {
			title = "Blocking greeting lines",
			description = "To block a certain dialogue line efficiently, it's best to check if the line passes first. Only if the line passes do the other checks. Since the game checks each line until it finds one which passes, your callback function registered on this event will be called many times. So, this early out will prevent the game from stalling."
		}
	}
}