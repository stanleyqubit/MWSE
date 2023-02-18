--[[
UnitWind - A unit testing framework for Morrowind Script Extender (MWSE)

See `unitwind.tests.lua` for examples of how to use UnitWind.
]]
---@class UnitWind
---@field enabled boolean? Whether to enable UnitWind. Defaults to true.
---@field highlight boolean? Whether to highlight output. Defaults to true.
---@field beforeAll fun(self: UnitWind)? A function to run before all tests are run.
---@field afterAll fun(self: UnitWind)? A function to run after all tests have been run.
---@field beforeEach fun(self: UnitWind)? A function to run before each test.
---@field afterEach fun(self: UnitWind)? A function to run after each test.
---@field exitAfter boolean? Whether to exit the game after all tests have been run. Defaults to false.
---@field outputFile file*? The file to write output to. If not defined, output will be written to mwse.log.
local UnitWind = {
    schema = {
        name = "UnitWind",
        fields = {
            enabled = { type = "boolean", default = true, required = false },
            highlight = { type = "boolean", default = true, required = false },
            beforeAll = { type = "function", required = false },
            afterAll = { type = "function", required = false },
            beforeEach = { type = "function", required = false },
            afterEach = { type = "function", required = false },
            exitAfter = { type = "boolean", default = false, required = false },
            outputFile = { type = "string", required = false },
            --internal
            totalTests = { type = "number", default = 0, required = false },
            testsPassed = { type = "number", default = 0, required = false },
            testsFailed = { type = "number", default = 0, required = false },
            completedTests = { type = "table", default = {}, required = false },
        }
    }
}
local ansicolors = require("logging.colors")
local validator = require "unitwind.validator"

--Construct a new UnitWind instance
---@return UnitWind
function UnitWind.new(data)
    local unitwind = table.deepcopy(data)
    --Validate and set defaults
    validator.validate(unitwind, UnitWind.schema)
    setmetatable(unitwind, UnitWind)
    UnitWind.__index = UnitWind
    unitwind:setOutputFile(unitwind.outputFile)
    return unitwind
end

--Log results to a file other than mwse.log
function UnitWind:setOutputFile(outputFile)
    if outputFile == nil or string.lower(outputFile) == "mwse.log" then
        self.outputFile = nil
    else
        local errMsg = "[ERROR] Logger:setLogLevel() - Not a valid outputFile (must be a string)"
        assert(type(outputFile) == "string", errMsg)
        self.outputFile = io.open(outputFile, "w")
    end
end

---Expect a result to be a certain value or type
---@return UnitWind.expects #An object with functions to perform expectations on the result
function UnitWind:expect(result)
    --Example usage: expect(1).toBe(1)
    local expectTypes = {
        toBe = function(expectedResult, isNot)
            if not self.enabled then return false end
            if (result == expectedResult) == isNot then
                error(string.format("Expected value to %sbe %s, got: %s.", isNot and "not " or "", expectedResult, result))
            end
            return true
        end,

        --Example usage: expect(1).toBeType("number")
        toBeType = function(expectedType, isNot)
            if not self.enabled then return false end
            local thisType = type(result)
            if (thisType == expectedType) == isNot then
                error(string.format("Expected type to %sbe %s, got: %s.", isNot and "not " or "", expectedType, thisType))
            end
            return true
        end,

        toFail = function(_, isNot)
            if not self.enabled then return false end
            local status, res = pcall(result)
            if not isNot then
                if status then
                    error(string.format("Expected function to fail, but instead returned %s", res or "nil"))
                end
            else
                if not status then
                    error(string.format("Expected function to succeed, but it failed with error: %s", res))
                end
            end
            return true
        end,
    }

    ---@class UnitWind.expects
    ---@field toBe fun(expectedResult: any, isNot: boolean?): boolean #Check if the result is equal to the expected value
    ---@field toBeType fun(expectedType: string, isNot: boolean?): boolean #Check if the result is of the expected type
    ---@field toFail fun(isNot: boolean?): boolean #Check if the result is a function that fails
    ---@field NOT UnitWind.expects.NOT #A table with the same functions as the parent, except they perform the reverse check
    local expects = {}

    ---@class UnitWind.expects.NOT
    ---@field toBe fun(expectedResult: any, isNot: boolean?): boolean #Check if the result is not equal to the expected value
    ---@field toBeType fun(expectedType: string, isNot: boolean?): boolean #Check if the result is not of the expected type
    ---@field toFail fun(isNot: boolean?): boolean #Check if the result is a function that succeeds
    expects.NOT = {}
    for expectType, func in pairs(expectTypes) do
        expects[expectType] = function(e)
            return func(e, false)
        end
        expects.NOT[expectType] = function(e)
            return func(e, true)
        end
    end
    return expects
end

function UnitWind:start(testsName)
    self.testsName = testsName or ""
    self:_logHeader("-----------------------------------------------------")
    self:_logHeader("Starting: %s", self.testsName)
    self:_logHeader("-----------------------------------------------------")
    if self.beforeAll then
        self:beforeAll()
    end
end

--Perform a test
function UnitWind:test(testName, callback)
    if not self.enabled then return end
    if self.beforeEach then
        self:beforeEach()
    end
    self.totalTests = self.totalTests + 1
    self:_log(self:_color("Running test: ", "cyan") .. testName)
    local status, error = pcall(callback)
    table.insert(self.completedTests, {
        name = testName,
        passed = status,
    })
    if status == true then
        self.testsPassed = self.testsPassed + 1
    else
        self.testsFailed = self.testsFailed + 1
        self:error("Error Message: %s\n%s", error, debug.traceback())
    end
    if self.afterEach then
        self:afterEach()
    end
end

--Reset the test count, restore mocks and print the results
function UnitWind:reset()
    self.testsName = ""
    self.totalTests = 0
    self.testsPassed = 0
    self.testsFailed = 0
    self.completedTests = {}
    self:restoreMocks()
end

--Finish all tests and print the results
function UnitWind:finish(exitAfter)
    if not self.enabled then return end
    self:_logHeader("-----------------------------------------------------")
    self:_logHeader("Finished: %s", self.testsName or "")
    self:_logHeader("-----------------------------------------------------")
    for _, test in ipairs(self.completedTests) do
        if test.passed then
            self:_passLog(test.name)
        else
            self:_failLog(test.name)
        end
    end
    self:_log("")
    local passed = self:_color(string.format("%d passed", self.testsPassed), 'green')
    local failed = self:_color(string.format("%d failed", self.testsFailed), self.testsFailed > 0 and 'red' or 'white')
    local total = string.format("%d total", self.totalTests)

    self:_log("%s, %s, %s", passed, failed, total)
    if self.testsPassed == self.totalTests then
        self:_log("")
        self:_logSuccess("✔️ %s PASSED ✔️", self.testsName:upper())
        self:_log("")
    else
        self:_log("")
        self:_logFailure("❌ %s FAILED ❌", self.testsName:upper())
        self:_log("")
    end
    self:_logHeader("-----------------------------------------------------")
    if self.afterAll then
        self:afterAll()
    end
    self:reset()
    if exitAfter or self.exitAfter then
        self:_log("Exiting Morrowind")
        os.exit(0)
    end
end

--Mock out a value on a table or module
---@param object table|string The table or module which contains the key to mock
---@param key string The function or value to mock
function UnitWind:mock(object, key, mockvalue)
    if type(object) == "string" then
        object = include(object)
        if not object then
            error(string.format("Could not find module to mock %s", object))
        end
    end
    local originalValue = object[key]
    self.mocks = self.mocks or {}
    self.mocks[object] = self.mocks[object] or {}
    self.mocks[object][key] = originalValue
    object[key] = mockvalue
end

--Restore a mock to its original state
---@param object table|string The table or module which contains the key to unmock
---@param key string The function or value to unmock
function UnitWind:unmock(object, key)
    if type(object) == "string" then
        object = include(object)
        if not object then
            error(string.format("Could not find module to unmock %s", object))
        end
    end
    local originalFunction = self.mocks[object][key]
    object[key] = originalFunction
    --remove from self.mocks
    self.mocks[object][key] = nil
    if not next(self.mocks[object]) then
        self.mocks[object] = nil
    end
end

--Restore all mocked functions to their original state
function UnitWind:restoreMocks()
    if not self.mocks then return end
    for object, mocks in pairs(self.mocks) do
        for key, originalValue in pairs(mocks) do
            object[key] = originalValue
        end
    end
    self.mocks = nil
end

------------------------------------------
-- Private functions
------------------------------------------

function UnitWind:_color(message, color)
    if self.highlight then
        message = ansicolors('%' .. string.format('{%s}%s', color, message))
    end
    return message
end

function UnitWind:_rawLog(message, ...)
    if not self.enabled then return end
    local prefix = self:_color("[UnitWind] ", 'bright magenta')
    local output = tostring(prefix .. message):format(...)
    --Prints to custom file if defined
    if self.outputFile then
        self.outputFile:write(output .. "\n")
        self.outputFile:flush()
    else
        --otherwise straight to mwse.log
        print(output)
    end
end

function UnitWind:_log(message, ...)
    self:_rawLog(message, ...)
end

function UnitWind:_logSuccess(message, ...)
    self:_rawLog(self:_color(message, 'greenbg black'), ...)
end

function UnitWind:_logFailure(message, ...)
    self:_rawLog(self:_color(message, 'redbg bright white'), ...)
end

function UnitWind:_logWhite(message, ...)
    self:_rawLog(self:_color(message, 'white'), ...)
end

function UnitWind:error(message, ...)
    self:_rawLog(self:_color(message, 'red'), ...)
end

function UnitWind:_logHeader(message, ...)
    self:_rawLog(self:_color(message, 'bright magenta'), ...)
end

function UnitWind:_passLog(message, ...)
    local pass = self:_color('✔️', 'green')
    self:_rawLog(pass .. " " .. message, ...)
end

function UnitWind:_failLog(message, ...)
    local fail = self:_color('❌', 'red')
    self:_rawLog(fail .. " " .. message, ...)
end

return UnitWind
