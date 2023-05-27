function tes3.isCharGenStarted()
	-- charGenState is 0 on the main menu.
	return tes3.worldController ~= nil and tes3.worldController.charGenState.value ~= 0
end

function tes3.isCharGenRunning()
	-- charGenState is set to 10 in vanilla, but this isn't guaranteed with mods, so instead check that it's > 0
	return tes3.worldController ~= nil and tes3.worldController.charGenState.value > 0
end

function tes3.isCharGenFinished()
	-- Vanilla sets charGenState to -1 once finished.
	return tes3.worldController ~= nil and tes3.worldController.charGenState.value < 0
end

local function onSimulate(e)
	if tes3.isCharGenFinished() then
		event.unregister(tes3.event.simulate, onSimulate)
		event.trigger(tes3.event.charGenFinished)
	end
end

local function onLoaded(e)
	if event.isRegistered(tes3.event.simulate, onSimulate) then
		event.unregister(tes3.event.simulate, onSimulate)
	end

	if e.newGame then
		event.register(tes3.event.simulate, onSimulate)
	end
end

event.register(tes3.event.loaded, onLoaded)
