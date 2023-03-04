return {
	type = "function",
	description = [[Returns an actor's current AI package ID, just as the mwscript function `GetCurrentAIPackage` would. The return value maps to values in `tes3.aiPackage` namespace.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3mobileActor|tes3reference" },
		},
	}},
	returns = "packageID",
	valuetype = "number",
}