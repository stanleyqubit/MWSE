--Common utils for registering dependency types
local util  = {}
local v = require("semver")

util.operators = {
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
util.defaultOperator = util.operators[1]
function util.getOperator(version)
    for _, operator in ipairs(util.operators) do
        if version:startswith(operator.pattern) then
            version = version:sub(#operator.pattern + 1)
            return operator, version
        end
    end
    return util.defaultOperator, version
end

function util.isValidUrl(url)
    url = url and url:lower()
    return url
       and url:startswith("http://")
        or url:startswith("https://")
end

function util.getMissingAssets(assets)
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

return util