local DependencyType = require("Metadata.DependencyType")
local Metadata = require("Metadata")
local inspect = require("inspect")

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
            version = version:sub(#operator.pattern + 1)
            return operator, version
        end
    end
    return defaultOperator, version
end

local isValidUrl = function(url)
    url = url and url:lower()
    return url
       and url:startswith("http://")
        or url:startswith("https://")
end

local function getDownloadButton(modId, dependency, text)
    if dependency.url then
        if not isValidUrl(dependency.url) then
            mwse.log(string.format("Invalid url for dependency %s: %s", modId, dependency.url))
            return false
        end
        return {
            text = string.format("%s %s", text or "Download", modId),
            tooltip = string.format('Go to "%s"', dependency.url),
            callback = function()
                local downloadExe = string.format('start %s', dependency.url)
                mwse.log("Executing command: %s", downloadExe)
                os.execute(downloadExe)
                os.exit()
            end
        }
    end
end

DependencyType.registerDependencyType{
    id = "mwse",
    checkDependency = function(_, dependency)
        if dependency.buildnumber > mwse.buildNumber then---@diagnostic disable-line: undefined-field
            return false, {
                {
                    title = "MWSE Update Required",
                    reasons = {
                        string.format("Current Build: %d; Required: %s",
                            mwse.buildNumber, dependency.buildnumber)---@diagnostic disable-line: undefined-field
                    },
                    resolveButton = {
                        text = "Update MWSE",
                        tooltip = "Run 'Morrowind/MWSE-Update.exe'",
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
                    title = "Mod: " .. modId,
                    reasons = {
                        string.format("Plugin %s is missing or inactive", dependency.plugin)
                    },
                }
                if dependency.url then
                    failure.resolveButton = getDownloadButton(modId, dependency)
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
                    title = "Mod: " .. modId,
                    reasons = {
                        string.format("Dependency %s not found", modId)
                    },
                }
                if dependency.url then
                    failure.resolveButton = getDownloadButton(modId, dependency)
                end
                dependencyManager.logger:error("Dependency %s not found", modId)
                table.insert(failures, failure)
            else
                local operator, dependencyVersion = getOperator(dependency.version)
                if not operator then
                    dependencyManager.logger:error("Dependency %s has invalid version requirement %s", modId, dependency.version)
                    table.insert(failures, {
                        title = "Mod: " .. modId,
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
                        title = "Mod: " .. modId,
                        reasons = {
                            error
                        },
                    })
                elseif not versionMatches then
                    local failure = {
                        title = "Mod: " .. modId,
                        reasons = {
                            string.format("Dependency %s is outdated.\n  (Current Version: %s; Required: %s)",
                                modId, modVersion, dependency.version)
                        },
                    }
                    if dependency.url then
                        failure.resolveButton = getDownloadButton(modId, dependency, "Update")
                    end
                    dependencyManager.logger:error("Dependency %s is outdated.\n  Current Version: %s; Required: %s.",
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
                    title = "Mod: " .. modId,
                    reasons = {
                        string.format("MWSE Module %s is missing", dependency["mwse-module"])
                    },
                }
                if dependency.url then
                    failure.resolveButton = getDownloadButton(modId, dependency)
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
    ---@param dependencyManager DependencyManager
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

local function getMissingAssets(assets)
    local missingAssets = {}
    for _, asset in ipairs(assets) do
        local fullPath = tes3.installDirectory .. "\\Data Files\\" .. asset
        local found = lfs.fileexists(fullPath)
                or lfs.directoryexists(fullPath)
        if not found then
            table.insert(missingAssets, asset)
        end
    end
    return missingAssets
end

DependencyType.registerDependencyType{
    id = "assets",
    ---@param dependencyManager DependencyManager
    ---@param assets string[]
    checkDependency = function(dependencyManager, assets)
        --Check assets exist as files or directories
        local missingAssets = getMissingAssets(assets)
        if #missingAssets > 0 then
            return false, {
                {
                    title = "Missing Assets",
                    reasons = missingAssets
                }
            }
        end
        return true
    end
}

local function isArchiveActive(archive)
    local loadedArchives = tes3.getArchiveList()
    for _, loadedArchive in ipairs(loadedArchives) do
        mwse.log("archive: " .. loadedArchive)
        if loadedArchive:lower() == "data files\\".. archive:lower() then
            mwse.log("FOUND ARCHIVE %s", archive)
            return true
        end
    end
    mwse.log("MISSING ARCHIVE %s", archive)
    return false
end

DependencyType.registerDependencyType{
    id = "archives",
    ---@param dependencyManager DependencyManager
    checkDependency = function(dependencyManager, archives)
        local failures = {}
        for archive, assets in pairs(archives) do
            local missingAssets = {}
            if not isArchiveActive(archive) then
                missingAssets = getMissingAssets(assets)
            end
            if #missingAssets > 0 then
                table.insert(failures, {
                    title = string.format("%s: Missing Assets\n  (Make sure to activate or unpack the archive)", archive),
                    reasons = missingAssets
                })
            end
        end
        if #failures > 0 then
            return false, failures
        end
        return true
    end
}