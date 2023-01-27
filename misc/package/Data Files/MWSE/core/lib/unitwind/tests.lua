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
local DO_FAILURE_TESTS = true

local UnitWind = require("unitwind")
local unitwind = UnitWind.new{
    enabled = true,
    highlight = true,
    exitAfter = true,
}

unitwind:start("Test Suite")

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

if DO_FAILURE_TESTS then
    unitwind:test("Lua assert fails", function()
        assert(false, "Lua assertion failed")
    end)
    unitwind:test("This test will fail", function()
        local x = 1
        unitwind:expect(x).toBe(2)
    end)
    unitwind:test("This test will fail to fail", function()
        unitwind:expect(function()
            return true
        end).toFail()
    end)
    unitwind:test("This test will fail to not fail", function()
        unitwind:expect(function()
            error("This is an error")
        end).NOT.toFail()
    end)
end

unitwind:finish()