return {
	type = "function",
	description = [[Configures MWSE to execute a given function instead when a script would run.

In most cases its intended to stop the execution of the original mwscript script. You can do so in the callback function by calling `mwscript.stopScript()`.]],
	arguments = {
		{ name = "scriptId", type = "string" },
		{ name = "callback", type = "function" },
	},
	returns = "boolean",
	examples = {
		["example"] = {
			title = "Here is an example of the most common use case for this function."
		}
	}
}