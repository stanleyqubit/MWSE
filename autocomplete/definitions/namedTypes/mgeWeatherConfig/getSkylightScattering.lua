return {
	type = "function",
	description = [[Gets the skylight scatter values from MGE. These are returned in a table with the `skylight` and `mix` keys. The result table can be modified, then sent back to `setSkylightScattering`.]],
	returns = {
		{ name = "scatters", type = "table<string, number[]|number>" },
	},
}
