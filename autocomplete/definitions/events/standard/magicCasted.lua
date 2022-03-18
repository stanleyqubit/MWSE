return {
	type = "event",
	description = "This event is triggered when any spell or enchant is cast successfully, or when any alchemy item is used. This includes spells cast via scripts. For spells, this occurs at the end of the casting animation. For spells and enchants, it is just after the magic projectile has been constructed.",
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the spell.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the spell. For self-targeted spells, this matches the caster. Touch spells can also have `target` parameter. On target spells don't have `target` parameter during this event, since it can't have a target until it hits it. Which isn't known when the spell is casted.",
		},
		["source"] = {
			type = "tes3alchemy|tes3enchantment|tes3spell",
			readOnly = true,
			description = "The magic source.",
		},
		["sourceInstance"] = {
			type = "tes3magicSourceInstance",
			readOnly = true,
			description = "The unique instance of the magic source.",
		},
	},
}