
# Mod Configuration Menu

Mods can create new GUIs. One GUI that that MWSE ships with is the Mod Configuration Menu. By registering into this system, mods can create settings pages for their mods in a place where users can easily find and manipulate them.

## Simplest Example

Below is the simplest code required to setup a mod config:

```lua linenums="1"
local configPath = "My Awesome Mod" -- The name of the config json file of your mod

-- The config table
local config = mwse.loadConfig(configPath, {
	enabled = true
})

-- When the mod config menu is ready to start accepting registrations, 
-- register this mod.
local function registerModConfig()
    -- Create the top level component Template
    -- The name will be displayed in the mod list on the lefthand pane
    local template = mwse.mcm.createTemplate({ name = "My Awesome Mod" })

    -- Save config options when the mod config menu is closed
	template:saveOnClose(configPath, config)

    -- Create a simple container Page under Template
	local settings = template:createPage({ label = "Settings" })

    -- Create a button under Page that toggles a variable between true and false
 	settings:createYesNoButton({
		label = "Enable Mod",
		variable = mwse.mcm:createTableVariable({ id = "enabled", table = config }),
	})

    -- Finish up.
	template:register()
end
event.register(tes3.event.modConfigReady, registerModConfig)
```

After loading up the game, you should see "My Awesome Mod" in the mod config menu, with a Yes/No button with label "Enable Mod" in its configuration pane.

## Advanced Examples

### Separate config.lua

As your default config gets long, you might want to create a separate Lua file for it. 

!!! example "**MyMod\\config.lua**"
	```lua linenums="1" 
	local configPath = "My Awesome Mod" -- The name of the config json file of your mod
	local defaultConfig = {
		enabled = true,
		keybind = {
			keyCode = tes3.scanCode.k,
			isShiftDown = false,
			isAltDown = false,
			isControlDown = true,
		}, -- Ctrl + K
		nonEdible = {
			["ingred_diamond_01"] = true, 
			["ingred_scrap_metal_01"] = true,
		}
	}

	return mwse.loadConfig(configPath, defaultConfig)
	```

And we import the config with `require` function in the `main.lua`.

!!! example "**MyMod\\main.lua**"
	```lua linenums="1" 
	local config = require("MyMod.config")
	```

### Side Bar Page

MCM has a lot of features and one of the most commonly-used components is the Side Bar Page. When a component is moused over in a side bar page, the right panel will display the description of that component. It is recommended to use a side bar page instead of a page when a mod has lots of settings options.

!!! example "**MyMod\\main.lua**"
	```lua
	local settings = template:createSideBarPage({ 
		label = "Settings",
		-- This description will be displayed at the right panel if no component is moused over.
		description =
            "My Awesome Mod\n" ..
            "Version 1.0.0\n" ..
            "\n" ..
            "The mod is very awesome. And this is its awesome description.\n",
    })

    settings:createYesNoButton{
        label = "Enable Mod",
		-- This description will be displayed at the right panel if the button is moused over.
        description =
            "If this setting is enabled, the mod is enabled.\n" ..
            "\n" ..
            "If this setting is disabled, the mod is disabled.\n" ..
            "\n" ..
            "Default: yes",
        variable = mwse.mcm:createTableVariable({ id = "enabled", table = config }),
    }
	```

### Key Binder

If your mod adds any hotkey function, a hotkey rebind in the mod config menu is a must. 

!!! example "**MyMod\\main.lua**"
	```lua
	settings:createKeyBinder{
		label = "Assign Keybind",
		description = "Assign a new keybind to perform awesome tasks.",
		variable = mwse.mcm.createTableVariable{ id = "keybind", table = config },
		allowCombinations = true,
	}
	```

### Exclusions Page

Let's say your My Awesome Mod makes certain ingredients non-edible. Using an Exclusions Page, you can allow users to manually add what ingredients should be non-edible in game.

!!! example "**MyMod\\main.lua**"
	```lua
	template:createExclusionsPage{
		label = "Non-Edible Ingredients",
		description = "Ingredients that can't be directly consumed via equipping.",
		leftListLabel = "Non-Edible Ingredients",
		rightListLabel = "Ingredients",
		variable = mwse.mcm.createTableVariable{
			id = "nonEdible",
			table = config,
		},
		-- This filters the right list by ingredient object
		filters = {
			{ label = "Ingredients", type = "Object", objectType = tes3.objectType.ingredient }, 
		},
	}
	```

There is also a more customized way to filter the right list. The following code filters the right list by ingredients with value higher than 100 gold.

!!! example "**MyMod\\main.lua**"
	```lua
		filters = {
			{	
				label = "Ingredients", 
				callback = function()
					local ingreds = {}
					---@param ingred tes3ingredient
					for ingred in tes3.iterateObjects(tes3.objectType.ingredients) do
						if ingred.value >= 100 then
							table.insert(ingreds, ingred)
						end
					end
					table.sort(merchants)
					return merchants
                end 
			}, 
		},
	```