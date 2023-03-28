return {
	type = "method",
	description = [[Returns the combined value of all the items worn by the actor.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "useDurability", type = "boolean", default = false, description = "If true, damaged items will have a proportionally lower value." },
		}},
	},
	returns = {
		{ name = "value", type = "number", description = "The value of each equipped item added together." },
	}
}
