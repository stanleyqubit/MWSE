return {
	type = "function",
	description = [[This function fetches a reference's attached animation groups. The animation groups match the values from [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) table.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference", description = "A reference whose animation groups to fetch." },
		},
	}},
	returns = {
		{ name = "lowerBodyGroup", type = "integer" },
		{ name = "upperBodyGroup", type = "integer" },
		{ name = "leftArmGroup", type = "integer" },
	}
}
