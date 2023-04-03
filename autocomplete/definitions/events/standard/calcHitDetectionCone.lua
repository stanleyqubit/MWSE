return {
	type = "event",
	description = "This event is raised before a hit attempt, to set up the hit detection cone of a attack swing. It is also raised a second time when the attack animation reaches its hit point, to check if the target is still within reach.",
	eventData = {
		["attacker"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference of the attacker.",
		},
		["attackerMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile which is making the attack.",
		},
		["reach"] = {
			type = "number",
			description = "The weapon's reach multiplier, typically in the range 0.5 - 2.0. May be adjusted.",
		},
		["angleXY"] = {
			type = "number",
			description = "The cosine of the maximum allowable angle offset in the horizontal direction that will hit. This is the same units as game setting fCombatAngleXY. May be adjusted.",
		},
		["angleZ"] = {
			type = "number",
			description = "The cosine of the maximum allowable angle offset in the vertical direction that will hit. This is the same units as game setting fCombatAngleXY. May be adjusted.",
		},
	},
	filter = "attacker.baseObject",
}
