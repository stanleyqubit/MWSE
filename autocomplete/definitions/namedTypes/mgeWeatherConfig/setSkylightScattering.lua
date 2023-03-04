return {
	type = "function",
	description = [[Sets the sky scatter values for MGE. The result table of `getSkylightScattering` can be modified then passed to this function.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "skylight", type = "tes3vector3|table" },
			{ name = "mix", type = "number", optional = true, default = 0.44 },
		},
	}},
}
