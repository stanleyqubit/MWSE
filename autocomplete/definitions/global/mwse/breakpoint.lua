return {
	type = "function",
	description = [[Prints "[MWSE] Hit breakpoint: ", with the provided `message` string appended, to the `mwse.log` file. This function is meant for debugging purposes.]],
	arguments = {
		{ name = "message", type = "string", optional = true,  description = "Provides a way to mark in the log which breakpoint was reached." },
	},
}