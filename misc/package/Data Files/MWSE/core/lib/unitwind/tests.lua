--[[
    Test suite for Unitwind itself.

    To generate the results of these tests yourself,
    call `require("unitwind.tests")` inside a main.lua file.

    --Expected Output--
    [UnitWind] -----------------------------------------------------
    [UnitWind] Starting: Test Suite
    [UnitWind] -----------------------------------------------------
    [UnitWind] Running test: Empty test
    [UnitWind] Running test: Test toBe
    [UnitWind] Running test: Test toBeType
    [UnitWind] Running test: Test NOT toBe
    [UnitWind] Running test: Test NOT toBeType
    [UnitWind] Running test: Test toFail
    [UnitWind] Running test: Test NOT toFail
    [UnitWind] Running test: Test mock
    [UnitWind] Running test: Lua assert fails
    [UnitWind] Error Message: .\Data Files\MWSE\lib\unitwind\tests.lua:109: Lua assertion failed
    stack traceback:
            .\Data Files\MWSE\lib\unitwind\init.lua:150: in function 'test'
            .\Data Files\MWSE\lib\unitwind\tests.lua:108: in function 'loader'
            Data Files\MWSE\core\mwse_init.lua:59: in function 'require'
            Data Files\MWSE\mods\_test\main.lua:2: in main chunk
    [UnitWind] Running test: This test will fail
    [UnitWind] Error Message: .\Data Files\MWSE\lib\unitwind\init.lua:69: Expected value to be 2, got: 1.
    stack traceback:
            .\Data Files\MWSE\lib\unitwind\init.lua:150: in function 'test'
            .\Data Files\MWSE\lib\unitwind\tests.lua:111: in function 'loader'
            Data Files\MWSE\core\mwse_init.lua:59: in function 'require'
            Data Files\MWSE\mods\_test\main.lua:2: in main chunk
    [UnitWind] Running test: This test will fail to fail
    [UnitWind] Error Message: .\Data Files\MWSE\lib\unitwind\init.lua:89: Expected function to fail, but instead returned true
    stack traceback:
            .\Data Files\MWSE\lib\unitwind\init.lua:150: in function 'test'
            .\Data Files\MWSE\lib\unitwind\tests.lua:115: in function 'loader'
            Data Files\MWSE\core\mwse_init.lua:59: in function 'require'
            Data Files\MWSE\mods\_test\main.lua:2: in main chunk
    [UnitWind] Running test: This test will fail to not fail
    [UnitWind] Error Message: .\Data Files\MWSE\lib\unitwind\init.lua:93: Expected function to succeed, but it failed with error: .\Data Files\MWSE\lib\unitwind\tests.lua:122: This is an error
    stack traceback:
            .\Data Files\MWSE\lib\unitwind\init.lua:150: in function 'test'
            .\Data Files\MWSE\lib\unitwind\tests.lua:120: in function 'loader'
            Data Files\MWSE\core\mwse_init.lua:59: in function 'require'
            Data Files\MWSE\mods\_test\main.lua:2: in main chunk
    [UnitWind] -----------------------------------------------------
    [UnitWind] Finished: Test Suite
    [UnitWind] -----------------------------------------------------
    [UnitWind] ✔️ Empty test
    [UnitWind] ✔️ Test toBe
    [UnitWind] ✔️ Test toBeType
    [UnitWind] ✔️ Test NOT toBe
    [UnitWind] ✔️ Test NOT toBeType
    [UnitWind] ✔️ Test toFail
    [UnitWind] ✔️ Test NOT toFail
    [UnitWind] ✔️ Test mock
    [UnitWind] ❌ Lua assert fails
    [UnitWind] ❌ This test will fail
    [UnitWind] ❌ This test will fail to fail
    [UnitWind] ❌ This test will fail to not fail
    [UnitWind]
    [UnitWind] 8 passed, 4 failed, 12 total
    [UnitWind]
    [UnitWind] ❌ TEST SUITE FAILED ❌
    [UnitWind]
    [UnitWind] -----------------------------------------------------
    [UnitWind] Exiting Morrowind
]]

--Do tests we expect to fail
local DO_POSITIVE_TESTS = true
local DO_NEGATIVE_TESTS = true

local UnitWind = require("unitwind")

local finishTestSuit = UnitWind.new{
    enabled = true,
    highlight = true,
    exitAfter = false,
}
local testObject = {
    testFunction = function()
        return true
    end
}
finishTestSuit:start("UnitWind:finish() test suite")
finishTestSuit:test("clearMocks", function()
    finishTestSuit:mock(testObject, "testFunction", function()
        return false
    end)
    finishTestSuit:expect(testObject.testFunction()).toBe(false) -- passed
    finishTestSuit:clearMocks()
    finishTestSuit:expect(testObject.testFunction()).toBe(true) -- passed
end)
finishTestSuit:finish()

local unitwind = UnitWind.new{
    enabled = true,
    highlight = true,
    exitAfter = true,
}

unitwind:start("Test Suite")

unitwind:test("Check that data from previous test has been cleared", function()
    unitwind:expect(unitwind.totalTests).toBe(1)
    unitwind:expect(unitwind.testsPassed).toBe(0)
    unitwind:expect(unitwind.testsFailed).toBe(0)
    unitwind:expect(table.size(unitwind.completedTests)).toBe(0)
end)

--Test previous testObject is correctly unmocked after `finish`
unitwind:test("Test mock from previous test suite was correctly reset on :finish()", function()
    unitwind:expect(testObject.testFunction()).toBe(true)
end)


if DO_POSITIVE_TESTS then
    unitwind:test("Empty test", function()
    end)
    unitwind:test("Test toBe", function()
        local x = 1
        unitwind:expect(x).toBe(1)
    end)
    unitwind:test("Test toBeType", function()
        local x = 1
        unitwind:expect(x).toBeType("number")
    end)
    unitwind:test("Test NOT toBe", function()
        local x = 1
        unitwind:expect(x).NOT.toBe(2)
    end)
    unitwind:test("Test NOT toBeType", function()
        local x = 1
        unitwind:expect(x).NOT.toBeType("string")
    end)
    unitwind:test("Test toFail", function()
        unitwind:expect(function()
            error("This is an error")
        end).toFail()
    end)
    unitwind:test("Test toFailWithError", function()
        unitwind:expect(function()
            error("This is an error")
        end).toFailWithError("This is an error")
    end)

    unitwind:test("Test NOT toFail", function()
        unitwind:expect(function()
            return true
        end).NOT.toFail()
    end)
    unitwind:test("Test mock", function()
        unitwind:mock(tes3, "player", {name = "Test Player"})
        unitwind:expect(tes3.player.name).toBe("Test Player")
        unitwind:unmock(tes3, "player")
    end)

    unitwind:test("Test expects.toBeCalled on a spy", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction()
        unitwind:expect(testObject.testFunction).toBeCalled()
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("Test expects.toBeCalled on a mock", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:mock(testObject, "testFunction", function()
            return true
        end)
        testObject.testFunction()
        unitwind:expect(testObject.testFunction).toBeCalled()
        unitwind:unmock(testObject, "testFunction")
    end)


    unitwind:test("Test expects.NOT.toBeCalled on a mock", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:mock(testObject, "testFunction", function()
            return true
        end)
        unitwind:expect(testObject.testFunction).NOT.toBeCalled()
        unitwind:unmock(testObject, "testFunction")
    end)

    unitwind:test("Test expects.toBeCalledTimes" , function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction()
        testObject.testFunction()
        unitwind:expect(testObject.testFunction).toBeCalledTimes(2)
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("Test expects.toBeCalledWith", function()
        local testObject = {
            testFunction = function(e)
                return e
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test")
        unitwind:expect(testObject.testFunction).toBeCalledWith{"test"}
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("Test expects.toBeCalledWith multiple arguments", function()
        local testObject = {
            testFunction = function(e, f)
                return e, f
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test", "test2")
        unitwind:expect(testObject.testFunction).toBeCalledWith{"test", "test2"}
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("Test toBeCalledWith using any()", function()
        local testObject = {
            testFunction = function(e, f)
                return e, f
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test", "test2")
        unitwind:expect(testObject.testFunction).toBeCalledWith{"test", UnitWind.any()}
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("test toBeCalledWith using any(type)", function()
        local testObject = {
            testFunction = function(e, f)
                return e, f
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test", "test2")
        unitwind:expect(testObject.testFunction).toBeCalledWith{"test", unitwind.any("string")}
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("Test expects.NOT.toBeCalledWith", function()
        local testObject = {
            testFunction = function(e)
                return e
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test")
        unitwind:expect(testObject.testFunction).NOT.toBeCalledWith{"test2"}
        unitwind:unspy(testObject, "testFunction")
    end)

    unitwind:test("Test unspying successfully removes from spies list", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:spy(testObject, "testFunction")
        unitwind:unspy(testObject, "testFunction")
        unitwind:expect(unitwind.spies[testObject]).toBe(nil)
    end)

    unitwind:test("Test unmocking successfully removes from mocks list", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:mock(testObject, "testFunction", function()
            return false
        end)
        unitwind:unmock(testObject, "testFunction")
        unitwind:expect(unitwind.mocks[testObject]).toBe(nil)
    end)

    unitwind:test("Test unmocking successfully removes from spies list", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:mock(testObject, "testFunction", function()
            return false
        end)
        unitwind:unmock(testObject, "testFunction")
        unitwind:expect(unitwind.spies[testObject]).toBe(nil)
    end)

    unitwind:test("Test clearMocks clears all mocks", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:mock(testObject, "testFunction", function()
            return false
        end)
        unitwind:expect(unitwind.mocks).NOT.toBe(nil)
        unitwind:expect(testObject.testFunction()).toBe(false)
        unitwind:clearMocks()
        unitwind:expect(unitwind.mocks).toBe(nil)
        unitwind:expect(testObject.testFunction()).toBe(true)
    end)
end

if DO_NEGATIVE_TESTS then
    unitwind:failTest("Test spy when spy target is nil", function()
        local testObject = {}
        unitwind:spy(testObject, "testFunction")
    end, "Key testFunction is not a function, got nil")

    unitwind:failTest("Test spy when target is not a function", function()
        local testObject = {
            testFunction = "Not a function"
        }
        unitwind:spy(testObject, "testFunction")
    end, "Key testFunction is not a function, got string")

    unitwind:failTest("Lua assert fails", function()
        assert(false, "Lua assertion failed")
    end, "Lua assertion failed")

    unitwind:failTest("This test will fail", function()
        local x = 1
        unitwind:expect(x).toBe(2)
    end, "Expected value to be 2, got: 1.")

    unitwind:failTest("This test will fail to fail", function()
        unitwind:expect(function()
            return true
        end).toFail()
    end, "Expected function to fail, but instead returned true")

    unitwind:failTest("This test will fail to not fail", function()
        unitwind:expect(function()
            error("This is an error")
        end).NOT.toFail()
    end)

    unitwind:failTest("Test expects.toBeCalled", function()
        local testObject = {
            testFunction = function()
                return true
            end
        }
        unitwind:spy(testObject, "testFunction")
        unitwind:expect(testObject.testFunction).toBeCalled()
    end, "Expected function to be called, but it was not called.")

    unitwind:failTest("Test expects.toBeCalledWith using literals", function()
        local testObject = {
            testFunction = function(e)
                return e
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test")
        unitwind:expect(testObject.testFunction).toBeCalledWith{"test2"}
    end, 'Expected function to be called with "test2", but it was called with "test".')

    unitwind:failTest('Test expects.toBeCalledWith using any("string")', function()
        local testObject = {
            testFunction = function(e, f)
                return e, f
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction(5, "test")
        unitwind:expect(testObject.testFunction).toBeCalledWith{unitwind.any("string"), "test"}
    end, 'Expected function to be called with any(string), "test", but it was called with 5, "test".')

    unitwind:failTest("Test expects.toBeCalledWith using any(number)", function()
        local testObject = {
            testFunction = function(e, f)
                return e, f
            end
        }
        unitwind:spy(testObject, "testFunction")
        testObject.testFunction("test", "test2")
        unitwind:expect(testObject.testFunction).toBeCalledWith{"test", unitwind.any("number")}
    end, 'Expected function to be called with "test", any(number), but it was called with "test", "test2".')

end

unitwind:finish()