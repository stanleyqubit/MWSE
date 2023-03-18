return {
	type = "method",
	deprecated = true,
	description = [[**Deprecated, please use [`applyDamage()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#applydamage) instead.**

Damages the actor.]],
	arguments = {
		{ name = "damage", type = "number", description = "The amount of damage to apply." },
		{ name = "isPlayerAttack", type = "boolean", optional = true, default = false, description = "Used to calculate difficulty scaling." },
		{ name = "scaleWithDifficulty", type = "boolean", optional = true, description = "Apply difficulty scaling to the damage." },
		{ name = "doNotChangeHealth", type = "boolean", optional = true, default = false, description = "Do all the audiovisual effects of damage without the health change." },
	},
	valuetype = "boolean",
}