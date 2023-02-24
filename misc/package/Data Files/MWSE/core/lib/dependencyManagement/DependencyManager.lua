local DependencyType = require("dependencyManagement.DependencyType")
---@class MWSE.Metadata.Package
---@field name string
---@field version string
---@field description string
---@field authors string[]

---@class MWSE.Metadata.Tools.MWSE
---@field lua-mod string The path to the main.lua associated with this mod
---@field load-order number The priority for when this mod is loaded. Lower numbers are loaded first.
---@field wait-until-initialize boolean Whether to wait until the game has initialized before loading this mod.

---@class MWSE.Metadata.Tools
---@field mwse MWSE.Metadata.Tools.MWSE

---Extend this when registering your own dependency type
---@class MWSE.Metadata.Dependency
---@field url string The url to download this dependency from

---@class MWSE.Metadata
---@field package MWSE.Metadata.Package
---@field tools MWSE.Metadata.Tools
---@field dependencies MWSE.Metadata.Dependency[]

---@class DependencyManager.new.params
---@field metadata MWSE.Metadata The metadata of the mod using this dependency manager
---@field logger mwseLogger? The logger to use for this dependency manager
---@field logLevel string? The log level to use for this dependency manager if no logger is supplied
---@field showFailureMessage boolean? Whether to show a message box if a dependency fails to load. Defaults to true.

--[[
    This class is used to manage dependencies for a mod.
    It will check if all dependencies are met, and if not
    it will display a message box to the user.
]]
---@class DependencyManager
---@field name string The name of the dependency manager
---@field metadata MWSE.Metadata The metadata of the mod using this dependency manager
---@field logger mwseLogger? The logger to use for this dependency manager
---@field showFailureMessage boolean?  Whether to show a message box if a dependency fails to load. Defaults to true.
---@field failedDependencies MWSE.DependencyType.Failure[]? The list of failed dependencies
local DependencyManager = {
    ---@type DependencyManager A list of all registered dependency managers
    registeredManagers = {},
}


--Construct a new Dependency Manager
---@param e DependencyManager.new.params
---@return DependencyManager
function DependencyManager.new(e)
    local self = setmetatable({}, { __index = DependencyManager })
    self.logger = e.logger
    self.metadata = e.metadata
    self.showFailureMessage = e.showFailureMessage == nil and true or e.showFailureMessage
    self.name = e.metadata
        and e.metadata.package
        and e.metadata.package.name
        and e.metadata.package.name .. ".DependencyManager"
        or "DependencyManager"
    if not self.logger then
        local MWSELogger = require("logging.logger")
        self.logger = MWSELogger.new {
            name = self.name,
            logLevel = e.logLevel or "INFO",
        }
    end
    self.logger:assert(type(self.metadata) == "table",
        "DependencyManager.new: metadata must be a table")
    self.logger:assert(type(self.metadata.package) == "table",
        "DependencyManager.new: metadata.package must be a table")
    return self
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
            self.logger:error("Dependencies failed to load, adding to registered managers")
            self.failedDependencies = failedDependencies
            table.insert(DependencyManager.registeredManagers, self)
        end
        return false
    end
    self.logger:debug("All dependencies met")
    return true
end

return DependencyManager
