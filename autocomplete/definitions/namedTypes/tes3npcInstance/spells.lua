return {
	type = "value",
	description = [[Quick access to the NPC's spell list. It is a `tes3spellList`, which is a list wrapper with helper functions. The actual list is iterated over using `pairs`. E.g. `for _, spell in pairs(npc.spells) do print(spell.name) end`]],-- Update description
	readOnly = true,
	valuetype = "tes3spellList|tes3spell[]",
}