return {
	type = "function",
	description = [[Saves a serializable table to `path`, using `toml.encode`. Unlike the related `json.savefile` function, this is relative to the current working directory, and not relative to Data Files\MWSE. A file extension should be part of the `path` string.]],
	arguments = {
		{ name = "path", type = "string", description = "The file to write to, relative to the current working directory (typically the Morrowind installation folder)." },
		{ name = "data", type = "table", description = "The data to encode and write." },
	},
}
