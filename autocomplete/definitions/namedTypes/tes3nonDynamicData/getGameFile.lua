return {
	type = "method",
	description = [[Returns a tes3gameFile for a given file name.]],
	arguments = {
		{ name = "fileName", type = "string", description = "The name of the file to get." },
	},
	returns = {
		{ name = "gameFile", type = "tes3gameFile|nil", description = "The game file associated with the name." },
	},
}
