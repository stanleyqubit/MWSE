
local myMenu -- Created before
---@cast myMenu tes3uiElement

local pane = myMenu:createVerticalScrollPane{ id = "sortedPane" }
pane:createButton{ text = "V" }
pane:createButton{ text = "S" }
pane:createButton{ text = "B" }
pane:getContentElement():sortChildren(function(a, b)
	return a.text < b.text
end)