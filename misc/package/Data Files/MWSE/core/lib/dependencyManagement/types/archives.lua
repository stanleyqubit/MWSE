local util = require("dependencyManagement.util")
local function isArchiveActive(archive)
    local loadedArchives = tes3.getArchiveList()
    for _, loadedArchive in ipairs(loadedArchives) do
        if loadedArchive:lower() == "data files\\".. archive:lower() then
            return true
        end
    end
    return false
end

return {
    id = "archives",
    checkDependency = function(_, archives)
        local failures = {}
        for archive, assets in pairs(archives) do
            local missingAssets = {}
            if not isArchiveActive(archive) then
                missingAssets = util.getMissingAssets(assets)
            end
            if #missingAssets > 0 then
                table.insert(failures, {
                    title = string.format("%s: Missing Assets\n  (Make sure to activate or unpack the archive)", archive),
                    reasons = missingAssets
                })
            end
        end
        if table.size(failures) > 0 then
            return false, failures
        end
        return true
    end
}