return {
	type = "function",
	description = [[Gets the value of an item and, optionally, an associated itemData. This can be useful if you wish to take durability and soul value into account. It will also take into account any installed Morrowind Code Patch rebalances. It can also be used to get the value of a reference.]],
	arguments = { {
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3item|string", optional = true, description = "The item to get the value of. Not needed if a reference is given." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data to use to modify the value. Not needed if a reference is given." },
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The reference to get the value of. Not used if an item is given." },
			{ name = "useDurability", type = "boolean", default = true, description = "If set to false, condition and uses will be ignored." },
			{ name = "useSoulValue", type = "boolean", default = true, description = "If set to false, the soul value will be ignored, effectively giving you the base soul gem value." },
		},
	} },
	returns = {
		{ name = "value", type = "number", description = "The calculated value of the item." },
	},
}
