return {
	type = "method",
	description = "Checks to get the number of items in the given inventory.",
	arguments = {
		{ name = "item", type = "tes3item|string", description = "The item to check for." },
	},
	returns = {
		{ name = "count", type = "number", description = "The number of the given item in the inventory."}
	},
}
