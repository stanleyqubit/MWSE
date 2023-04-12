return {
	type = "value",
	description = [[Quick access to the creature's spell list. It is a `tes3spellList`, which is a list wrapper with helper functions. The actual list is iterated over using `pairs`. E.g. `for _, spell in pairs(creature.spells) do print(spell.name) end`]],
	readOnly = true,
	valuetype = "tes3spellList|tes3spell[]",
}