return {
	type = "function",
	description = [[Loads the contents of a file through json.decode. Files loaded from Data Files\\MWSE\\{fileName}.json.

!!! warning
	If the table encoded as json had both string and integer indices, this process converted all the integer indices to strings. For example, `[1]` was converted to `["1"]`. So, when loading mixed tables from json, this needs to be considered. If you need to save and load your mod's configuration file, consider using [`mwse.loadConfig()`](https://mwse.github.io/MWSE/apis/mwse/#mwseloadconfig) as that function will do this for you.
]],
	arguments = {
		{ name = "fileName", type = "string" },
	},
	valuetype = "table",
}