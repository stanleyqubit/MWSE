return {
	type = "function",
	description = [[Determines whether a key is pressed. A wrapper for `GetAsyncKeyState` function in Win32 API.]],
	arguments = {
		{ name = "VK_key", type = "integer" },
	},
	returns = "boolean",
}