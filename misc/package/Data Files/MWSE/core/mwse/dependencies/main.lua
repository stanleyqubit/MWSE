local DependencyType = require("dependencyManagement.DependencyType")
local DependencyNotifier = require("dependencyManagement.DependencyNotifier")
local DependencyManager = require("dependencyManagement.DependencyManager")
local LOG_LEVEL = "INFO"
local logger = require("logging.logger").new{
    name = "Register Dependency Types",
    logLevel = LOG_LEVEL
}
local function isLuaFile(file) return file:sub(-4, -1) == ".lua" end
local function isInitFile(file) return file == "init.lua" end

--[[
    Register all dependency types in the dependencyManagement.types folder.
]]
local modulePath = "dependencyManagement.types.?"
local path = "Data Files/MWSE/core/lib/dependencyManagement/types"
logger:debug("Registering dependency types from: %s", path)
for file in lfs.dir(path) do
    logger:debug("Registering dependency type: %s", file)
    if isLuaFile(file) and not isInitFile(file) then
        logger:debug("Is lua file: %s", file)
        local module = modulePath:gsub("%?", file:sub(1, -5))
        logger:debug("Module: %s", module)
        local dependencyConfig = require(module)
        dependencyConfig.logLevel = LOG_LEVEL
        logger:debug("Registering dependency type: %s", dependencyConfig.id)
        DependencyType.registerDependencyType(dependencyConfig)
    end
end

--[[
    Once all mods have been initialized, this function will be called
    to display a message box for each dependency manager that has failed dependencies.
]]
event.register(tes3.event.initialized, function()
    local function showNextMessage()
        local manager = table.remove(DependencyManager.registeredManagers, 1)
        if manager and manager.failedDependencies then
            local notifier = DependencyNotifier:new {
                logger = manager.logger,
                failedDependencies = manager.failedDependencies,
                packageName = manager.metadata
                    and manager.metadata.package
                    and manager.metadata.package.name
            }
            notifier:dependenciesFailMessage(showNextMessage)
        end
    end
    showNextMessage()
end, { priority = -math.huge })