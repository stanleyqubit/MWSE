---@type tes3uiElement
local menu -- Created somewhere before

local button = menu:createButton{text = "- Item -"}
local item = tes3.getObject("iron_shield") --[[@as tes3armor]]

button:register("help", function(e)
	local tooltip = tes3ui.createTooltipMenu{item = item}
	tooltip:createLabel{text = "Additional custom text"}
end)
