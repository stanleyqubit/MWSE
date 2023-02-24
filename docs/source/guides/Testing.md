# Testing

MWSE comes with a unit testing framework called `UnitWind`. This allows you to create and run tests that verify the behavior of your code. Your tests can be configured to run when Morrowind starts, then exit Morrowind when they finish. This allows you to run your tests instantly and repeatedly.

## Initializing UnitWind

Create a new UnitWind instance like so:

```lua
local unitWind = require("unitwind").new{
    enabled = true,
    --- ... other settings ...
}
```

UnitWind can be configured with the following settings:

* `enabled` - boolean (default: true)

    Whether to enable UnitWind.

* `highlight` - boolean (default: true)

    Whether to highlight output. If for some reason you're reading the output in a text editor instead of the console, you may want to disable this.

* `beforeAll` - function(self)

    A function to run before all tests are run. Useful for setting up common test data or adding mocks.

* `afterAll` - function(self)

    A function to run after all tests have been run. Useful for cleaning up common test data or mocks.

* `beforeEach` - function(self)

    A function to run before each test. Useful for setting up test data or mocks.

* `afterEach` - function(self)

    A function to run after each test. Useful for cleaning up test data or mocks.

* `exitAfter` - boolean (default: false)

    Whether to exit the game after all tests have been run.

* `outputFile` - string (default: mwse.log)

    The file to write output to. If not defined, output will be written to mwse.log.

## Creating a Test Suite

A test suite begins with a call to `unitwind:start(testSuiteName)` and ends with a call to `unitwind:finish()`. You can create multiple test suites in a single UnitWind instance.

Here is an example of a simple test suite:

```lua
local unitwind = require("unitwind").new{
    enabled = true,
    exitAfter = true,
}
unitwind:start("My Example Test Suite")
unitwind:test("Test 1 + 1 = 2", function()
    unitwind:expect(1 + 1).toBe(2)
end)
unitwind:finish()
```

The output of this test suite will look like this:

    [UnitWind] -----------------------------------------------------
    [UnitWind] Starting: My Example Test Suite
    [UnitWind] -----------------------------------------------------
    [UnitWind] Running test: Test 1 + 1 = 2
    [UnitWind] -----------------------------------------------------
    [UnitWind] Finished: My Example Test Suite
    [UnitWind] -----------------------------------------------------
    [UnitWind] ✔️ Test 1 + 1 = 2
    [UnitWind]
    [UnitWind] 1 passed, 0 failed, 1 total
    [UnitWind]
    [UnitWind] ✔️ MY EXAMPLE TEST SUITE PASSED ✔️
    [UnitWind]
    [UnitWind] -----------------------------------------------------
    [UnitWind] Exiting Morrowind[UnitWind] -----------------------------------------------------

## Fail Tests



## Expect

The `expect` function is used to make various assertions about the behavior of your code. It takes a single argument, which is the value to test. It returns an object with a number of methods that can be used to make assertions about the value.

### Expect Methods

* `toBe`

    Asserts that the value is equal to the expected value. This uses the `==` operator, so it will not work for tables. Use `toEqual` instead.

    ```lua
    unitwind:expect(1 + 1).toBe(2)
    ```

* `toBeType`

    Asserts that the value is of the expected type.

    ```lua
    unitwind:expect(1 + 1).toBeType("number")
    ```

* `toFail`

    Asserts that the function passed to it will fail when called.

    ```lua
    unitwind:expect(function()
        error("This should fail")
    end).toFail()
    ```
* `toFailWithError`

    Asserts that the function passed to it will fail when called, and that the error message will match the expected error message.

    Example:
    ```lua
    unitwind:expect(function()
        error("This should fail")
    end).toFailWithError("This should fail")
    ```

    Note: For errors thrown with line numbers, only the error messaage after the line number will be compared.

    For example, the error: `"Data Files\MWSE\core\initialize.lua:45: error loading module"`
    would be tested using
    ```
    toFailWithError("error loading module")
    ```

* `toBeCalled`

    Asserts that the function passed to it will be called. The function must be mocked or spied on (See the Mocking and Spying section below for more information).

    ```lua
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
    ```

* `toBeCalledTimes`

    Asserts that the function passed to it will be called a specific number of times. The function must be mocked or spied on (See the Mocking and Spying section below for more information).

    ```lua
    local testObject = {
        testFunction = function()
            return true
        end
    }
    unitwind:mock(testObject, "testFunction", function()
        return true
    end)
    testObject.testFunction()
    testObject.testFunction()
    unitwind:expect(testObject.testFunction).toBeCalledTimes(2)
    unitwind:unmock(testObject, "testFunction")
    ```

* `toBeCalledWith`

    Asserts that the function passed to it will be called with the expected arguments. The function must be mocked or spied on (See the Mocking and Spying section below for more information).

    When testing with multiple arguments, pass it to `toBeCalledWith` as a table.

    ```lua
    local testObject = {
        testFunction = function()
            return true
        end
    }
    unitwind:mock(testObject, "testFunction", function()
        return true
    end)
    testObject.testFunction("test", 1)
    unitwind:expect(testObject.testFunction).toBeCalledWith{"test", 1}
    unitwind:unmock(testObject, "testFunction")
    ```

* `NOT`

    The `expects.NOT` object can be used to negate any of the above methods. For example, if you want to assert that a function will not fail, you can do so like this:

    ```lua
    unitwind:expect(function()
        return true
    end).NOT.toFail()
    ```

## Mocking and Spying

Mocking and Spying are two ways to intercept function calls. Mocking allows you to replace a function with a custom function, while spying allows you to intercept a function call without replacing it. In both cases, you can use the `expect` function to make assertions about the function calls, such as whether it was called or how many times it was called.

### Mocking

To mock a function, use the `mock` function. It takes three arguments:

* `object` - the object that contains the function to mock. This can be a table or a string. If it is a string, it will be passed to `include` to get the module to mock.
* `functionName` - the name of the function to mock
* `mockFunction` - the function to replace the original function with

Example:
```lua
local testObject = {
    testFunction = function()
        return true
    end
}
unitwind:mock(testObject, "testFunction", function()
    return false
end)
```

A more practical example is to mock out tes3 objects which aren't available when the game first launches, such as tes3.player. This allows you to test functions that rely on game objects without having to load a save game.

Example:
```lua
unitwind:mock(tes3, "player", {
    name = "Test Player",
    position = tes3vector3.new(0, 0, 0),
})
```

#### Unmocking

It is always a good idea to unmock a function after you are done with it. This will restore the original function. To unmock a function, use the `unmock` function. It takes two arguments:

* `object` - the object that contains the function to unmock. This can be a table or a string. If it is a string, it will be passed to `include` to get the module to unmock.
* `functionName` - the name of the function to unmock

Example:
```lua
unitwind:unmock(tes3, "player")
```

You can also unmock all functions by calling `clearMocks`:

```lua
unitwind:clearMocks()
```

Mocks are cleared automatically when `unitwind:finish()` is called.

### Spying

Spying lets you track function calls without replacing the function. To spy on a function, use the `spy` function. It takes two arguments:

* `object` - the object that contains the function to spy on. This can be a table or a string. If it is a string, it will be passed to `include` to get the module to spy on.
* `functionName` - the name of the function to spy on

Example:
```lua
local testObject = {
    testFunction = function()
        return true
    end
}
unitwind:spy(testObject, "testFunction")
```

To remove a spy from a function, use the `unspy` function:
```lua
unitwind:unspy(testObject, "testFunction")
```

You can also remove all spies by calling `clearSpies`:
```lua
unitwind:clearSpies()
```

Spies are cleared automatically when `unitwind:finish()` is called.