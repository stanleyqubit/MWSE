return {
	type = "function",
	description = [[This function fetches a reference's attached animation groups' timings.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "A reference whose animation groups' timings to fetch." },
		},
	}},
	returns = { name = "result", type = "number[]" }, -- TODO: change to type = "number[3]", blocked by: https://github.com/LuaLS/lua-language-server/issues/1081
	examples = {
		["usage"] = {
			title = "An elegent usage example",
			description = [[The function returns animation timings for three body segments. This array can be nicely broken down into three variables using Lua's `unpack()` function.]]
		}
	}
}
