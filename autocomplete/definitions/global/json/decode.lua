return {
	type = "function",
	description = [[Decode string into a table.

!!! warning
	If the table encoded as json had both string and integer indices, this process converted all the integer indices to strings. For example, `[1]` was converted to `["1"]`. So, when loading mixed tables from json, this needs to be considered.
]],
	link = "http://dkolf.de/src/dkjson-lua.fsl/wiki?name=Documentation",
	arguments = {
		{ name = "s", type = "string" },
		{ name = "position", type = "number", optional = true, default = 1 },
		{ name = "nullValue", type = "string|nil", optional = true, default = "nil" },
	},
	valuetype = "table",
}