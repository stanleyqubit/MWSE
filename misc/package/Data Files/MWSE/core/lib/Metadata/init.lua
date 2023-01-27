---@class MWSE.Metadata.Package
---@field name string The name of the mod.
---@field description string The description of the mod.
---@field homepage string A URL to the mod's homepage.
---@field repository string A URL to the mod's repository.
---@field authors string[] The authors of the mod.
---@field version string The current mod version.

---@class MWSE.Metadata
---@field package MWSE.Metadata.Package
---@field dependencies table<string, table> A table of dependencies used by the mod.
local Metadata = {}

---@return MWSE.Metadata|nil
function Metadata.getMetadata(modId)
    local path = string.format("Data Files\\%s-metadata.toml", modId)
    local file = io.open(path, "r")
    if file == nil then
        return nil
    end
    local contents = file:read("*all")
    local metadata = toml.decode(contents)
    file:close()

    assert(metadata ~= nil, string.format("Failed to parse metadata for mod %s.", modId))
    assert(type(metadata.package) == "table", string.format("Failed to parse package metadata for mod %s.", modId))
    assert(type(metadata.package.name) == "string", string.format("Failed to parse package name for mod %s.", modId))
    if metadata.dependencies then
        for dependencyId, dependency in pairs(metadata.dependencies) do
            assert(type(dependencyId) == "string", string.format("Failed to parse dependency id for mod %s.", modId))
            assert(type(dependency) == "table", string.format("Failed to parse dependency for mod %s.", modId))
         end
        end
    return metadata
end



return Metadata