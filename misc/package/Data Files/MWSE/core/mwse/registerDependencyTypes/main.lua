local DependencyType = require("Metadata.DependencyType")
local Metadata = require("Metadata")

local v = require("semver")
local operators = {
    {
        pattern = "=",
        func = function(a, b) return v(a) == v(b) end,
    },
    {
        pattern = ">=",
        func = function(a, b) return v(a) <= v(b) end,
    },
    {
        pattern = "<=",
        func = function(a, b) return v(a) >= v(b) end,
    },
    {
        pattern = ">",
        func = function(a, b) return v(a) < v(b) end,
    },
    {
        pattern = "<",
        func = function(a, b) return v(a) > v(b) end,
    }
}
local defaultOperator = operators[1]
local function getOperator(version)
    for _, operator in ipairs(operators) do
        if version:startswith(operator.pattern) then
            mwse.log("Operator: %s", operator.pattern)
            version = version:sub(#operator.pattern + 1)
            mwse.log("Version: %s", version)
            return operator, version
        end
    end
    return defaultOperator, version
end

DependencyType.registerDependencyType{
    id = "mwse",
    checkDependency = function(dependencyManager, dependency)
        if dependency.buildnumber > mwse.buildNumber then---@diagnostic disable-line: undefined-field
            return false, {
                {
                    title = "MWSE Update Required",
                    reasons = {
                        string.format("Current Build: %d; Required: %s.",
                            mwse.buildNumber, dependency.buildnumber)---@diagnostic disable-line: undefined-field
                    },
                    resolveButton = {
                        text = "Update MWSE",
                        callback = function()
                            local updateExe = "start .\\MWSE-Update.exe"
                            os.execute(updateExe)
                            os.exit()
                        end
                    }
                }
            }
        end
        return true
    end
}

local function doPluginCheck(dependencyManager, mods, failures)
    for modId, dependency in pairs(mods) do
        assert(type(modId) == "string", "Mod id must be a string")
        assert(type(dependency) == "table", "Dependency must be a table")
        --check plugin
        if dependency.plugin then
            dependencyManager.logger:debug("Checking plugin %s", dependency.plugin)
            local isActive = tes3.isModActive(dependency.plugin)
            if not isActive then

                local failure = {
                    title = modId,
                    reasons = {
                        string.format("Plugin %s is missing or inactive", dependency.plugin)
                    },
                }
                if dependency.url then
                    failure.resolveButton = {
                        text = string.format("Download %s", modId),
                        callback = function()
                            local downloadExe = "start " .. dependency.url
                            os.execute(downloadExe)
                            os.exit()
                        end
                    }
                end
                dependencyManager.logger:error("Plugin %s is missing or inactive", dependency.plugin)
                table.insert(failures, failure)
            end
        end
    end
    return failures
end

local function doVersionCheck(dependencyManager, mods, failures)
    for modId, dependency in pairs(mods) do
         --check version
         if dependency.version then
            dependencyManager.logger:debug("Dependency version %s", dependency.version)
            local metadata = Metadata.getMetadata(modId)
            local modVersion = metadata and metadata.package and metadata.package.version
            if not modVersion then
                local failure = {
                    title = modId,
                    reasons = {
                        string.format("Dependency %s not found", modId)
                    },
                }
                if dependency.url then
                    failure.resolveButton = {
                        text = string.format("Download %s", modId),
                        callback = function()
                            local downloadExe = "start " .. dependency.url
                            os.execute(downloadExe)
                            os.exit()
                        end
                    }
                end
                dependencyManager.logger:error("Dependency %s not found", modId)
                table.insert(failures, failure)
            else
                local operator, dependencyVersion = getOperator(dependency.version)
                if not operator then
                    dependencyManager.logger:error("Dependency %s has invalid version requirement %s", modId, dependency.version)
                    table.insert(failures, {
                        title = modId,
                        reasons = {
                            string.format("Dependency %s has invalid version requirement %s", modId, dependency.version)
                        },
                    })
                end
                local versionMatches
                local success, error = pcall(function()
                    dependencyManager.logger:debug("Checking version %s %s %s", dependencyVersion, operator.pattern, modVersion)
                    versionMatches = operator.func(dependencyVersion, modVersion)
                end)
                if not success then
                    dependencyManager.logger:error(error)
                    table.insert(failures, {
                        title = modId,
                        reasons = {
                            error
                        },
                    })
                elseif not versionMatches then
                    local failure = {
                        title = modId,
                        reasons = {
                            string.format("Dependency %s is outdated.\nCurrent Version: %s; Required: %s.",
                                modId, modVersion, dependency.version)
                        },
                    }
                    if dependency.url then
                        failure.resolveButton = {
                            text = string.format("Update %s", modId),
                            callback = function()
                                local downloadExe = "start " .. dependency.url
                                os.execute(downloadExe)
                                os.exit()
                            end
                        }
                    end
                    dependencyManager.logger:error("Dependency %s is outdated.\nCurrent Version: %s; Required: %s.",
                        modId, modVersion, dependency.version)
                    table.insert(failures, failure)
                end
            end
        end
    end
end

local function doModuleCheck(dependencyManager, mods, failures)
    for modId, dependency in pairs(mods) do
        if dependency["mwse-module"] then
            dependencyManager.logger:debug("Checking mwse module %s", dependency["mwse-module"])
            local path = dependency["mwse-module"]:gsub("[/.]", "\\"):lower()
            local packagePaths = package.path:gsub("%?%.lua", "?")
            local function checkModule(packagePath)
                local fullPath = packagePath:gsub("?", path)
                dependencyManager.logger:debug("Checking module %s", fullPath)
                return lfs.fileexists(tes3.installDirectory .. fullPath .. ".lua")
                    or lfs.directoryexists(tes3.installDirectory .. fullPath)
            end
            local found = false
            for packagePath in packagePaths:gmatch("[^;]+") do
                if checkModule(packagePath) then
                    found = true
                    break
                end
            end
            if not found then
                local failure = {
                    title = modId,
                    reasons = {
                        string.format("MWSE Module %s is missing", dependency["mwse-module"])
                    },
                }
                if dependency.url then
                    failure.resolveButton = {
                        text = string.format("Download %s", modId),
                        callback = function()
                            local downloadExe = "start " .. dependency.url
                            os.execute(downloadExe)
                            os.exit()
                        end
                    }
                end
                dependencyManager.logger:error("MWSE Module %s is missing", dependency["mwse-module"])
                table.insert(failures, failure)
            else
                dependencyManager.logger:debug("MWSE Module %s found", dependency["mwse-module"])
            end
        end
    end
end

DependencyType.registerDependencyType{
    id = "mods",
    logLevel = "DEBUG",
    checkDependency = function(dependencyManager, mods)
        local failures = {}
        doPluginCheck(dependencyManager, mods, failures)
        doVersionCheck(dependencyManager, mods, failures)
        doModuleCheck(dependencyManager, mods, failures)
        if #failures > 0 then
            return false, failures
        end
        return true
    end
}