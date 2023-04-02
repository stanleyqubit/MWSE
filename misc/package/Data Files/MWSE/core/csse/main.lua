
--
-- Load parameters passed by CSSE.
--

local commandLine = os.getCommandLine()
if (not table.find(commandLine, "--fromCSSE")) then
	return
end

-- CSSE should make a toml file for us to load. Get it and error out if it doesn't exist.
local csse_toml = toml.loadFile("csse.toml")
if (csse_toml == nil) then
	mwse.log("[CSSE] Could not load csse data to change Morrowind execution!")
	return
end

local test_environment = csse_toml.test_environment
if (test_environment == nil) then
	mwse.log("[CSSE] No test environment exists!")
	return
end

event.trigger("csse:environmentSetup", { environment = test_environment })
mwse.log("[CSSE] Loading test environment: %s", json.encode(test_environment, { indent = true }))


--
-- Overwrite the loading files to only load the files active in the CS.
--

-- Flag which files we want to load based on what we were given from CSSE, rather than loading from Morrowind.ini.
local function flagLoadFiles()
	for _, gameFile in ipairs(test_environment.game_files) do
		local file = tes3.dataHandler.nonDynamicData:getGameFile(gameFile)
		if (file == nil) then
			error(string.format("[CSSE] Could not locate game file for testing: %s", gameFile))
		end

		file.toLoad = true
	end
end

-- Do we even have any files to load?
if (#test_environment.game_files > 0) then
	mwse.memory.writeFunctionCall({ address = 0x419F53, call = flagLoadFiles, length = 0x419FE1 - 0x419F53 })
else
	mwse.log("[CSSE] No game files set to load!")
end


--
-- If start_new_game is set, automatically load into position.
--

local function GetStartingCellFromIni()
	mwse.memory.setThreadSafeStringBuffer(test_environment.starting_cell)
end

local function GetStartingGridX()
	return test_environment.starting_grid[1]
end

local function GetStartingGridY()
	return test_environment.starting_grid[2]
end

if (test_environment.start_new_game) then
	if (test_environment.starting_cell and test_environment.starting_cell ~= "") then
		mwse.memory.writeBytes({
			address = 0x41A0DD,
			bytes = {
				0x89, 0x98, 0x2C, 0x03, 0x00, 0x00, -- mov [eax+WorldController.startingCell], ebx
			},
		})

		mwse.memory.writeFunctionCall({
			address = 0x41A0DD + 0x6,
			call = GetStartingCellFromIni,
			length = 0x41A0FF - (0x41A0DD + 0x6),
		})
	else
		mwse.memory.writeFunctionCall({
			address = 0x41A253,
			call = GetStartingGridX,
			signature = { returns = "int" },
			length = 0x41A265 - 0x41A253,
		})
		mwse.memory.writeFunctionCall({
			address = 0x41A265,
			call = GetStartingGridY,
			signature = { returns = "int" },
			length = 0x41A275 - 0x41A265,
		})
		mwse.memory.writeNoOperation({ address = 0x41A277, length = 0x2 })
	end
end


--
-- When we've finished loading, set up the environment with the desired items/spells/journals/topics.
--

--- Ensure that environment is set up when we load in.
--- @param e loadedEventData
local function onLoaded(e)
	if (not e.newGame) then
		return
	end

	for journal, index in pairs(test_environment.journal or {}) do
		local object = tes3.findDialogue({ topic = journal, type = tes3.dialogueType.journal })
		if (object) then
			tes3.setJournalIndex({ id = journal, index = index, showMessage = false })
		else
			mwse.log("[CSSE] Could not find journal: %s", journal)
		end
	end

	for _, topic in ipairs(test_environment.topics or {}) do
		local object = tes3.findDialogue({ topic = topic, type = tes3.dialogueType.topic })
		if (object) then
			tes3.addTopic({ topic = topic })
		else
			mwse.log("[CSSE] Could not find topic: %s", topic)
		end
	end

	for item, count in pairs(test_environment.inventory or {}) do
		local object = tes3.getObject(item)
		if (object) then
			tes3.addItem({ reference = tes3.player, item = item, count = count, playSound = false })
		else
			mwse.log("[CSSE] Could not find item: %s", item)
		end
	end

	for _, spell in ipairs(test_environment.spells or {}) do
		local object = tes3.getObject(spell) --- @cast object tes3spell
		if (object and object.objectType == tes3.objectType.spell) then
			tes3.addSpell({ reference = tes3.player, spell = object })
		else
			mwse.log("[CSSE] Could not find spell: %s", spell)
		end
	end

	for global, value in pairs(test_environment.globals or {}) do
		local object = tes3.findGlobal(global)
		if (object) then
			object.value = value
		else
			mwse.log("[CSSE] Could not find global: %s", global)
		end
	end

	tes3.positionCell({
		reference = tes3.player,
		cell = tes3.player.cell,
		position = test_environment.position,
		orientation = test_environment.orientation,
	})

	event.trigger("csse:loaded", { environment = test_environment })

	mwse.log("[CSSE] Environment injection complete.")
end
event.register(tes3.event.loaded, onLoaded, { doOnce = true })
