
local skillBonusesF = {
	[tes3.skill.alchemy] = 15,
	[tes3.skill.illusion] = 25,
	[tes3.skill.mysticism] = -5,
}

local skillBonusesM = {
	[tes3.skill.athletics] = 10,
	[tes3.skill.unarmored] = 10,
	[tes3.skill.spear] = 15,
}

event.register(tes3.event.calcChargenStats, function()
    local skills = tes3.player.baseObject.female and skillBonusesF or skillBonusesM
    for k, v in pairs(skills) do
	    tes3.modStatistic{reference = tes3.player, skill = k, value = v}
    end
end)