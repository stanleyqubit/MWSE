local DependencyType = require("Metadata.DependencyType")
---@class DependencyManager.new.params
---@field metadata MWSE.Metadata The metadata of the mod using this dependency manager
---@field logger mwseLogger? The logger to use for this dependency manager
---@field showFailureMessage boolean? Whether to show a message box if a dependency fails to load. Defaults to true.

--[[
    This class is used to manage dependencies for a mod.
    It will check if all dependencies are met, and if not
    it will display a message box to the user.
]]
---@class DependencyManager
---@field metadata MWSE.Metadata The metadata of the mod using this dependency manager
---@field logger mwseLogger? The logger to use for this dependency manager
---@field showFailureMessage boolean?  Whether to show a message box if a dependency fails to load. Defaults to true.
---@field failedDependencies MWSE.DependencyType.Failure[]? The list of failed dependencies
local DependencyManager = {
    ---@type DependencyManager A list of all registered dependency managers
    registeredManagers = {},
}

--[[
    Once all mods have been initialized, this function will be called
    to display a message box for each dependency manager that has failed dependencies.
]]
event.register(tes3.event.initialized, function()
    local function showNextMessage()
        local manager = table.remove(DependencyManager.registeredManagers, 1)
        if manager and manager.failedDependencies then
            manager:dependenciesFailMessage(showNextMessage)
        end
    end
    showNextMessage()
end, { priority = -math.huge })


--Construct a new Dependency Manager
---@param e DependencyManager.new.params
---@return DependencyManager
function DependencyManager.new(e)
    local self = setmetatable({}, { __index = DependencyManager })
    self.logger = e.logger
    self.metadata = e.metadata
    self.showFailureMessage = e.showFailureMessage == nil and true or e.showFailureMessage

    if not self.logger then
        local name = e.metadata
            and e.metadata.package
            and e.metadata.package.name
            and e.metadata.package.name .. ".DependencyManager"
            or "DependencyManager"
        local MWSELogger = require("logging.logger")
        self.logger = MWSELogger.new {
            name = name,
            logLevel = "INFO"
        }
    end
    self.logger:assert(type(self.metadata) == "table",
        "DependencyManager.new: metadata must be a table")
    self.logger:assert(type(self.metadata.package) == "table",
        "DependencyManager.new: metadata.package must be a table")
    return self
end

---Display the list of failed dependencies to the player
---@param callback function The callback to call when the message box is closed
function DependencyManager:dependenciesFailMessage(callback)
    timer.frame.delayOneFrame(function()
        local function customBlock(parentBlock)
            parentBlock.widthProportional = 1.0
            parentBlock.borderTop = 6
            parentBlock.minWidth = 400
            --parentBlock.childAlignX = 0.5
            parentBlock:getTopLevelMenu():getContentElement().maxWidth = nil
            local scrollPane = parentBlock:createVerticalScrollPane()
            scrollPane.widthProportional = 1.0
            scrollPane.heightProportional = 1.0
            scrollPane.paddingAllSides = 2
            scrollPane.minHeight = 500

            for _, failure in pairs(self.failedDependencies) do
                local dependencyBlock = scrollPane:createThinBorder()
                dependencyBlock.flowDirection = "top_to_bottom"
                dependencyBlock.autoHeight = true
                dependencyBlock.widthProportional = 1.0
                dependencyBlock.paddingAllSides = 8
                dependencyBlock.paddingBottom = 12
                dependencyBlock.borderAllSides = 2
                --dependencyBlock.childAlignX = 0.5

                local title = failure.title or "NO TITLE"
                local titleLabel = dependencyBlock:createLabel { text = title }
                titleLabel.color = tes3ui.getPalette(tes3.palette.headerColor)
                titleLabel.wrapText = true
                titleLabel.justifyText = "center"
                titleLabel.widthProportional = 1.0
                titleLabel.borderBottom = 4
                titleLabel.color = tes3ui.getPalette(tes3.palette.headerColor)

                for _, reason in pairs(failure.reasons) do
                    local depLabel = dependencyBlock:createLabel { text = "- " .. reason }
                    --depLabel.color = tes3ui.getPalette(tes3.palette.negativeColor)
                    depLabel.wrapText = true
                    --depLabel.justifyText = "center"
                    depLabel.widthProportional = 1.0
                end
                if failure.resolveButton then
                    self.logger:assert(type(failure.resolveButton.text) == "string",
                        "DependencyManager:dependenciesFailMessage: resolveButton.text must be a string")
                    self.logger:assert(type(failure.resolveButton.callback) == "function",
                        "DependencyManager:dependenciesFailMessage: resolveButton.callback must be a function")
                    local button = dependencyBlock:createButton { text = failure.resolveButton.text }
                    button.widthProportional = 1.0
                    button.borderTop = 10
                    button:register("mouseClick", failure.resolveButton.callback)
                    if failure.resolveButton.tooltip then
                        button:register("help", function()
                            local tooltip = tes3ui.createTooltipMenu()
                            tooltip:createLabel{ text = failure.resolveButton.tooltip}
                        end)
                    end
                end
            end
            scrollPane.widget:contentsChanged()
            parentBlock:getTopLevelParent():updateLayout()
            parentBlock:updateLayout()
        end
        local buttons = {
            {
                text = tes3.findGMST(tes3.gmst.sOK).value,
                callback = function()
                    if callback then
                        callback()
                    end
                end,
            },
        }
        tes3ui.showMessageMenu {
            header = string.format("%s - Failed Dependencies", self.metadata.package.name),
            customBlock = customBlock,
            buttons = buttons,
        }
    end)
end

---Check if all dependencies are met
---@return boolean #returns true if all dependencies passed, false if any failed.
function DependencyManager:checkDependencies()
    self.logger:debug("Checking dependencies for: %s", self.metadata.package.name)
    local failedDependencies = {} ---@type table<string, MWSE.DependencyType.Failure>
    if self.metadata.dependencies then
        for typeId, dependency in pairs(self.metadata.dependencies) do
            assert(type(typeId) == "string", "DependencyManager:checkDependencies: typeId must be a string")
            assert(type(dependency) == "table", "DependencyManager:checkDependencies: dependency must be a table")
            self.logger:debug("Checking dependency: %s", typeId)
            self.logger:debug("dependency: %s", json.encode(dependency))
            local dependencyType = DependencyType.getDependencyType(typeId)
            if dependencyType then
                self.logger:debug("Checking dependency type: %s", typeId)
                local passed, failures = dependencyType:checkDependency(dependency)
                if failures and not passed then
                    self.logger:error("Dependency failed: %s", typeId)
                    for _, failure in pairs(failures) do
                        table.insert(failedDependencies, failure)
                    end
                end
            else
                self.logger:warn("Unknown dependency type: %s", typeId)
            end
        end
    end
    if table.size(failedDependencies) > 0 then
        if self.showFailureMessage then
            self.failedDependencies = failedDependencies
            table.insert(DependencyManager.registeredManagers, self)
        end
        return false
    end
    self.logger:debug("All dependencies met")
    return true
end

return DependencyManager
