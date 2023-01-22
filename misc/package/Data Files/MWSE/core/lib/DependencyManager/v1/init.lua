---@class DependencyManager.Dependency
---@field name string The name of the dependency
---@field luaFile string The path to the lua file to check for
---@field versionFile string The path to the version file to check for
---@field metadataFile string The path to the metadata file to check for
---@field version string The version to check for

---@class DependencyManager.new.params
---@field modName string The name of the mod using this dependency manager
---@field dependencies DependencyManager.Dependency[] The dependencies to check for
---@field logger MWSELogger (optional)The logger to use for this dependency manager
---@field showFailureMessage boolean (optional) Whether to show a message box if a dependency fails to load. Defaults to true.

---@class DependencyManager
---@field modName string The name of the mod using this dependency manager
---@field dependencies DependencyManager.Dependency[] The dependencies to check for
---@field logger MWSELogger The logger to use for this dependency manager
---@field showFailureMessage boolean (optional) Whether to show a message box if a dependency fails to load. Defaults to true.
local DependencyManager = {}

---@param e DependencyManager.new.params
---@return DependencyManager
function DependencyManager.new(e)
    local self = setmetatable({}, { __index = DependencyManager })
    self.logger = e.logger
    if not self.logger then
        local name = e.modName and e.modName .. ".DependencyManager" or "DependencyManager"
        local MWSELogger = require("logging.logger")
        self.logger = MWSELogger.new {
            name = name,
            logLevel = "INFO"
        }
    end
    self.logger:assert(type(e.modName) == "string", "modName must be a string")
    self.logger:assert(type(e.dependencies) == "table", "dependencies must be a table")

    self.modName = e.modName
    self.dependencies = e.dependencies
    self.showFailureMessage = e.showFailureMessage == nil and true or e.showFailureMessage
    return self
end

DependencyManager.operators = {
    [">"] = function(major, minor, patch, targetMajor, targetMinor, targetPatch)
        if major > targetMajor then
            return true
        elseif major == targetMajor then
            if minor > targetMinor then
                return true
            elseif minor == targetMinor then
                if patch > targetPatch then
                    return true
                end
            end
        end
        return false
    end,
    [">="] = function(major, minor, patch, targetMajor, targetMinor, targetPatch)
        if major > targetMajor then
            return true
        elseif major == targetMajor then
            if minor > targetMinor then
                return true
            elseif minor == targetMinor then
                if patch >= targetPatch then
                    return true
                end
            end
        end
        return false
    end,
    ["="] = function(major, minor, patch, targetMajor, targetMinor, targetPatch)
        if major == targetMajor and minor == targetMinor and patch == targetPatch then
            return true
        end
        return false
    end
}

function DependencyManager:dependencyFailMessage(dependency, currentVersion)
    timer.frame.delayOneFrame(function()
        local message = string.format("Required by: %s\nRequirement: %s", self.modName, dependency.name)
        if dependency.version then
            message = string.format("%s\nRequired Version: %s", message, dependency.version)
        end
        if currentVersion then
            message = string.format("%s\nCurrent Version: %s", message, currentVersion)
        end
        tes3ui.showMessageMenu{
            header = "Dependency Failure",
            message = message,
            buttons = {
                {
                    text = "Okay",
                    callback = function()
                        tes3ui.leaveMenuMode()
                    end,
                    showRequirements = function()
                        return dependency.url == nil
                    end
                },
                {
                    text = string.format("Download %s", dependency.name),
                    callback = function()
                        os.execute("start " .. dependency.url)
                        os.exit()
                    end,
                    showRequirements = function()
                        return dependency.url ~= nil
                    end
                },
                {
                    text = "Cancel",
                    callback = function()
                        tes3ui.leaveMenuMode()
                    end,
                    showRequirements = function()
                        return dependency.url ~= nil
                    end
                }
            }
        }
    end)
end

function DependencyManager:doLuaFileCheck(dependency)
    local luaFile = include(dependency.luaFile)
    if luaFile == nil then
        self.logger:error("Could not find dependency file: %s", dependency.luaFile)
        return false
    else
        if dependency.version then
            local version = luaFile.version
            if not version then
                self.logger:error("Could not find version in dependency file: %s", dependency.luaFile)
                return false
            end
            if not self:doVersionCheck(dependency, version) then
                return false
            end
        end
    end
    return true
end

function DependencyManager:doVersionCheck(dependency, version)
    if dependency.version == nil then
        --no version required
        return true
    end
    if version == nil then
        -- version required but none found
        return false
    end
    local major, minor, patch = string.match(version, "(%d+)%.(%d+)%.(%d+)")
    self.logger:debug("Found version: Major: %s, Minor: %s, Patch: %s", major, minor, patch)
    local targetMajor, targetMinor, targetPatch = string.match(dependency.version, "(%d+)%.(%d+)%.(%d+)")

    local operator
    --find one of possible operators at start of string
    for operatorPattern, _ in pairs(self.operators) do
        if string.startswith(dependency.version, operatorPattern) then
            operator = operatorPattern
            break
        end
    end
    if not operator then
        operator = "="
    end

    self.logger:debug("Operator: %s, Target: Major: %s, Minor: %s, Patch: %s", operator, targetMajor, targetMinor, targetPatch)
    if not self.operators[operator](major, minor, patch, targetMajor, targetMinor, targetPatch) then
        return false
    end
    return true
end

function DependencyManager:getFile(path)
    local fullPath = string.format("Data Files/MWSE/%s", path)
    local file = io.open(fullPath, "r")
    if not file then
        self.logger:error("Could not find dependency file: %s", fullPath)
        return nil
    end
    return file
end

function DependencyManager:doMetadataCheck(dependency)
    local metadataFile = self:getFile(dependency.metadataFile)
    if not metadataFile then
        self.logger:error("Could not find dependency metadata file: %s", dependency.metadataFile)
        return false
    else
        local metadata = json.decode(metadataFile:read("*all"))
        if not metadata then
            self.logger:error("Could not parse dependency metadata file: %s", dependency.metadataFile)
            return false
        end
        if not metadata.version then
            self.logger:debug("Could not find version in dependency metadata file: %s", dependency.metadataFile)
            return true
        end
        return self:doVersionCheck(dependency, metadata.version), metadata.version
    end
end

function DependencyManager:doVersionFileCheck(dependency)
    local versionFile = self:getFile(dependency.versionFile)
    if not versionFile then
        self.logger:error("Could not find dependency version file: %s", dependency.versionFile)
        return false
    else
        local version = ""
        for line in versionFile:lines() do -- Loops over all the lines in an open text file
            version = line
        end
        if version == "" then
            self.logger:error("Could not find version in dependency version file: %s", dependency.versionFile)
            return true
        end
        return self:doVersionCheck(dependency, version), version
    end
end

---@return boolean #returns true if all dependencies passed, false if any failed.
function DependencyManager:checkDependencies()
    for _, dependency in pairs(self.dependencies) do
        self.logger:debug("Checking dependency: %s", dependency.name)
        local passed
        local currentVersion
        if dependency.luaFile then
            passed, currentVersion = self:doLuaFileCheck(dependency)
        elseif dependency.versionFile then
            passed, currentVersion = self:doVersionFileCheck(dependency)
        elseif dependency.metadataFile then
            passed, currentVersion = self:doMetadataCheck(dependency)
        else
            self.logger:error("Dependency %s has no file path", dependency.name)
            return true
        end
        if not passed then
            if self.showFailureMessage then
                self:dependencyFailMessage(dependency, currentVersion)
            end
            return false
        end
    end
    self.logger:debug("All dependencies met")
    return true
end

return DependencyManager
