return {
	type = "function",
	description = [[Returns a random element from the given table.]],
	arguments = {
		{ name = "t", type = "table" },
	},
	returns = {
		{ name = "value", type = "unknown", description = "The randomly chosen value." },
		{ name = "key", type = "unknown", description = "The table key of the chosen value." }
	}
}