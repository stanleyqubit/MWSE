local plural = {}
local defaultFunction = nil
-- helper functions

local function assertPresentString(functionName, paramName, value)
  if type(value) ~= 'string' or #value == 0 then
    local msg = "Expected param %s of function %s to be a string, but got %s (a value of type %s) instead"
    error(msg:format(paramName, functionName, tostring(value), type(value)))
  end
end

local function assertNumber(functionName, paramName, value)
  if type(value) ~= 'number' then
    local msg = "Expected param %s of function %s to be a number, but got %s (a value of type %s) instead"
    error(msg:format(paramName, functionName, tostring(value), type(value)))
  end
end

-- transforms "foo bar baz" into {'foo','bar','baz'}
local function words(str)
  local result, length = {}, 0
  str:gsub("%S+", function(word)
    length = length + 1
    result[length] = word
  end)
  return result
end

local function isInteger(n)
  return n == math.floor(n)
end

local function between(value, min, max)
  return value >= min and value <= max
end

local function inside(v, list)
  for i=1, #list do
    if v == list[i] then return true end
  end
  return false
end


-- pluralization functions
-- note: language codes changed to ISO 639-2

local pluralization = {}

local f1 = function(n)
  return n == 1 and "one" or "other"
end
pluralization[f1] = {'deu', 'eng'}

local f5 = function(n)
  if not isInteger(n) then return 'other' end
  local n_10, n_100 = n % 10, n % 100
  return (n_10 == 1 and n_100 ~= 11 and 'one') or
         (between(n_10, 2, 4) and not between(n_100, 12, 14) and 'few') or
         ((n_10 == 0 or between(n_10, 5, 9) or between(n_100, 11, 14)) and 'many') or
         'other'
end
pluralization[f5] = {'rus'}

local f9 = function(n)
  return (n >= 0 and n < 2 and 'one') or
         'other'
end
pluralization[f9] = {'fra'}

local f21 = function(n)
  if n == 1 then return 'one' end
  if not isInteger(n) then return 'other' end
  local n_10, n_100 = n % 10, n % 100

  return ((between(n_10, 2, 4) and not between(n_100, 12, 14)) and 'few') or
         ((n_10 == 0 or n_10 == 1 or between(n_10, 5, 9) or between(n_100, 12, 14)) and 'many') or
         'other'
end
pluralization[f21] = {'pol'}

local pluralizationFunctions = {}
for f,locales in pairs(pluralization) do
  for _,locale in ipairs(locales) do
    pluralizationFunctions[locale] = f
  end
end

-- public interface

function plural.get(locale, n)
  assertPresentString('i18n.plural.get', 'locale', locale)
  assertNumber('i18n.plural.get', 'n', n)

  local f = pluralizationFunctions[locale] or defaultFunction

  return f(math.abs(n))
end

function plural.setDefaultFunction(f)
  defaultFunction = f
end

function plural.reset()
  defaultFunction = pluralizationFunctions['eng']
end

plural.reset()

return plural
