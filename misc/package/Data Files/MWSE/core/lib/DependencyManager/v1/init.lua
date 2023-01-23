---@class DependencyManager.Dependency
---@field name string The name of the dependency
---@field luaMod string The lua mod to check for. This should be a path to the folder containing the lua code/metadata for this mod
---@field plugin string The plugin file (esp, esm etc) of the mod to check for. This can be in addition to a lua/version/metadata file check
---@field version string The version to check for. It will look for a version file or metadata file

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
    {
        pattern = ">=",
        callback = function(major, minor, patch, targetMajor, targetMinor, targetPatch)
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
    },
    {
        pattern = ">",
        callback = function(major, minor, patch, targetMajor, targetMinor, targetPatch)
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
    },

    {
        pattern = "=",
        callback = function(major, minor, patch, targetMajor, targetMinor, targetPatch)
            if major == targetMajor and minor == targetMinor and patch == targetPatch then
                return true
            end
            return false
        end
    },
}


function DependencyManager:dependencyFailMessage(failedDependencies)
    timer.frame.delayOneFrame(function()
        local function customBlock(parentBlock)
            parentBlock.widthProportional = 1.0
            parentBlock.minWidth = 400
            parentBlock.childAlignX = 0.5
            parentBlock:getTopLevelMenu():getContentElement().maxWidth = nil
            for _, failedDependency in pairs(failedDependencies) do
                local dependency = failedDependency.dependency
                local dependencyBlock = parentBlock:createThinBorder()
                dependencyBlock.flowDirection = "top_to_bottom"
                dependencyBlock.autoHeight = true
                dependencyBlock.widthProportional = 1.0
                dependencyBlock.paddingAllSides = 6
                dependencyBlock.borderAllSides = 2
                dependencyBlock.childAlignX = 0.5
                local nameLabel = dependencyBlock:createLabel{ text = dependency.name }
                nameLabel.wrapText = true
                nameLabel.justifyText = "center"
                nameLabel.widthProportional = 1.0
                nameLabel.color = tes3ui.getPalette(tes3.palette.headerColor)
                for _, reason in pairs(failedDependency.reasons) do
                    local depLabel = dependencyBlock:createLabel{ text = "- " .. reason }
                    depLabel.color = tes3ui.getPalette(tes3.palette.negativeColor)
                    depLabel.wrapText = true
                    depLabel.justifyText = "center"
                    depLabel.widthProportional = 1.0
                end
                if dependency.url then
                    local button = dependencyBlock:createButton{ text = "Open Download Page" }
                    button:register("mouseClick", function(e)
                        os.openURL(dependency.url)
                        os.exit()
                    end)
                end
            end
            parentBlock:getTopLevelParent():updateLayout()
            parentBlock:updateLayout()
        end
        local buttons = {
            {
                text = tes3.findGMST(tes3.gmst.sCancel).value,
                callback = function()
                    tes3ui.leaveMenuMode()
                end,
            },
        }
        tes3ui.showMessageMenu{
            header = "The following dependencies failed to load:",
            customBlock = customBlock,
            buttons = buttons,
        }
    end)
end

function DependencyManager:doVersionCheck(dependency, version)
    if dependency.version == nil then
        --no version required
        return true
    end
    if version == nil then
        -- version required but none found
        return false, string.format("Could not find version field in lua file: %s", dependency.luaFile)
    end
    self.logger:debug("Version: %s", version)
    local major, minor, patch = string.match(version, "(%d+)%.(%d+)%.(%d+)")
    self.logger:debug("Split version: Major: %s, Minor: %s, Patch: %s", major, minor, patch)
    if not ( major and minor and patch ) then
        return false, string.format("Invalid version: %s", version)
    end
    local targetMajor, targetMinor, targetPatch = string.match(dependency.version, "(%d+)%.(%d+)%.(%d+)")

    local thisOperator
    --find one of possible operators at start of string
    self.logger:debug("Required version: %s", dependency.version)
    for _, operator in ipairs(self.operators) do
        self.logger:debug("operator pattern: %s", operator.pattern)
        if string.startswith(dependency.version, operator.pattern) then
            thisOperator = operator
            break
        end
    end
    if not thisOperator then
        self.logger:debug("No operator found, setting to '='")
        thisOperator = self.operators[3]
    end

    self.logger:debug("Operator: %s, Target: Major: %s, Minor: %s, Patch: %s", thisOperator.pattern, targetMajor, targetMinor, targetPatch)
    if not ( targetMajor and targetMinor and targetPatch ) then
        return false, string.format("Invalid version: %s", dependency.version)
    end
    if not thisOperator.callback(major, minor, patch, targetMajor, targetMinor, targetPatch) then
        return false, string.format("Version needed: %s; Current version: %s", dependency.version, version)
    end
    return true
end

function DependencyManager:doMetadataCheck(dependency, metadataPath)
    local metadataFile = io.open(metadataPath, "r")
    if not metadataFile then
        self.logger:error("Could not find dependency metadata file: %s", metadataPath)
        return false, string.format("Could not find dependency metadata file: %s", metadataPath)
    else
        local metadata = json.decode(metadataFile:read("*all"))
        if not metadata then
            self.logger:error("Could not parse dependency metadata file: %s", metadataPath)
            return false, string.format("Could not parse dependency metadata file: %s", metadataPath)
        end
        if not metadata.version then
            self.logger:debug("Could not find version in dependency metadata file: %s", metadataPath)
            return false, string.format("Could not find version in dependency metadata file: %s", metadataPath)
        end
        return self:doVersionCheck(dependency, metadata.version)
    end
end

function DependencyManager:doVersionFileCheck(dependency, versionFilePath)
    local versionFile = io.open(versionFilePath, "r")
    if not versionFile then
        self.logger:error("Could not find dependency version file: %s", versionFilePath)
        return false, string.format("Could not find dependency version file: %s", versionFilePath)
    else
        local version = ""
        for line in versionFile:lines() do -- Loops over all the lines in an open text file
            version = line
        end
        if version == "" then
            self.logger:error("Could not find version in dependency version file: %s", versionFilePath)
            return false, string.format("Could not find version in dependency version file: %s", versionFilePath)
        end
        return self:doVersionCheck(dependency, version)
    end
end

---@param dependency DependencyManager.Dependency
function DependencyManager:doLuaModCheck(dependency)

    local path = dependency.luaMod:gsub("[/.]", "\\"):lower()
    local packagePaths = package.path:gsub("%?%.lua", "?")

    for packagePath in packagePaths:gmatch("[^;]+") do
        local fullPath = packagePath:gsub("?", path .. "\\")
        self.logger:debug("checking path: %s", fullPath)

        -- if no version is specific, just check the folder exists
        if dependency.version == nil then
            self.logger:debug("No target version, check if '%s' folder exists", fullPath)
            local folderExists = lfs.directoryexists( tes3.installDirectory .. fullPath)
            if folderExists then
                return true
            else
                self.logger:debug("Folder does not exist: %s", fullPath)
                return false, string.format("Could not find dependency folder: %s", fullPath)
            end
        end

        --Look for metadata
        local metadataPath = fullPath .. "metadata.json"
        -- .\\Data Files\\MWSE\\core\\mods\\Ashfall\\
        local metadataExists = lfs.fileexists( tes3.installDirectory .. metadataPath)
        -- C:\\Game\\Morrowind\\Data Files\\MWSE\\core\\mods\\Ashfall\\metadata.json
        if metadataExists then
            return self:doMetadataCheck(dependency, metadataPath)
        end

        --Look for version file
        local versionFilePath = fullPath .. "version.txt"
        local versionFileExists = lfs.fileexists( tes3.installDirectory .. versionFilePath)
        if versionFileExists then
            return self:doVersionFileCheck(dependency, versionFilePath)
        end

        --Look for version in init.lua by using `include` and checking version field in returned table
        local init = include(dependency.luaMod)
        if init and init.version then
            return self:doVersionCheck(dependency, init.version)
        end
    end

    return false, string.format("Could not find dependency: %s", dependency.luaMod)
end

---@return boolean #returns true if all dependencies passed, false if any failed.
function DependencyManager:checkDependencies()
    local failedDependencies = {}
    local didFail = false
    for _, dependency in pairs(self.dependencies) do
        self.logger:debug("Checking dependency: %s", dependency.name)
        if dependency.plugin then
            self.logger:debug("Checking plugin dependency: %s", dependency.plugin)
            if not tes3.isModActive(dependency.plugin) then
                failedDependencies[dependency.name] = failedDependencies[dependency.name] or {
                    dependency = dependency,
                    reasons = {}
                }
                table.insert(failedDependencies[dependency.name].reasons,
                    string.format('Plugin "%s" is not active', dependency.plugin))
                didFail = true
            end
        end
        if dependency.luaMod then
            self.logger:debug("Checking luaMod dependency: %s", dependency.luaMod)
            local passed, reason = self:doLuaModCheck(dependency)
            if not passed then
                self.logger:error("Failed with reason: %s", reason)
                failedDependencies[dependency.name] = failedDependencies[dependency.name] or {
                    dependency = dependency,
                    reasons = {}
                }
                table.insert(failedDependencies[dependency.name].reasons, reason)
                didFail = true
            end
        elseif dependency.version then
            --version and no luaMod to check, throw error
            self.logger:error("Dependency has version but no luaMod to check: %s", dependency.name)
            failedDependencies[dependency.name] = failedDependencies[dependency.name] or {
                dependency = dependency,
                reasons = {}
            }
            table.insert(failedDependencies[dependency.name].reasons,
                string.format('Dependency has version but no luaMod to check: %s', dependency.name))
            didFail = true
        end
    end
    if didFail then
        if self.showFailureMessage then
            self:dependencyFailMessage(failedDependencies)
        end
        return false
    end
    self.logger:debug("All dependencies met")
    return true
end

return DependencyManager
