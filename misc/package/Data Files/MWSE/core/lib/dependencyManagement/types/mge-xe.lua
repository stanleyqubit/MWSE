---MGE XE Dependency - checks the version of MGE XE
---@class MWSE.Metadata.Dependency.MGE-XE : MWSE.Metadata.Dependency
---@field version string The version of MGE XE to check for. Can be a specific version, or a comparison operator and version. E,g ">= 1.0.0"

local util = require("dependencyManagement.util")

local reasons = {
    notInstalled = function(_)
        return {
            {
                title = "MGE XE is not installed",
                reasons = {
                    "MGE XE is not installed"
                }
            }
        }
    end,
    invalidTargetVerion = function(targetVersion)
        return {
            {
                title = "Invalid MGE XE version",
                reasons = {
                    string.format("The version specified for MGE XE is invalid: %s", targetVersion)
                }
            }
        }
    end,
    outOfDateVersion = function(targetVersion, mgeVersion)
        return {
            {
                title = "MGE XE is out of date",
                reasons = {
                    string.format("Current: %s; Required: %s", mgeVersion, targetVersion)
                }
            }
        }
    end,
    versionError = function(err)
        return {
            {
                title = "Error Parsing MGE Version",
                reasons = {
                    err
                }
            }
        }
    end
}

return {
    id = "mge-xe",
    logLevel = "DEBUG",
    ---@param dependencyManager DependencyManager
    ---@param mgeDependency MWSE.Metadata.Dependency.MGE-XE
    checkDependency = function(dependencyManager, mgeDependency)
        if not mgeDependency.version then return true end
        local mgeVersionTable = mge.getVersion()
        if not mgeVersionTable then
            return false, reasons.notInstalled()
        end
        local mgeVersion
        if type(mgeVersionTable) == "string" then
            mgeVersion = mgeVersionTable
        else
            mgeVersion = string.format("%d.%d.%d",
                mgeVersionTable.major,
                mgeVersionTable.minor,
                mgeVersionTable.patch
            )
        end
        local operator, targetVersion = util.getOperator(mgeDependency.version)
        if not operator then
            return false, reasons.invalidTargetVerion(targetVersion)
        end
        local passesVersionCheck
        local success, err = pcall(function()
            dependencyManager.logger:debug("Checking version %s %s %s", targetVersion, operator.pattern, mgeVersion)
            passesVersionCheck = operator.func(targetVersion, mgeVersion)
        end)
        if not success then
            dependencyManager.logger:error(err) ---@diagnostic disable-line: param-type-mismatch
            return false, reasons.versionError(err)
        end
        if not passesVersionCheck then
            return false, reasons.outOfDateVersion(targetVersion, mgeVersion)
        end
    end
}