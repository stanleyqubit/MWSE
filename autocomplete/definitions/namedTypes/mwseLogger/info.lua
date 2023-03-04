return {
	type = "method",
	description = [[Log info message.]],
	arguments = {
		{ name = "message", type = "string" },
		{ name = "formatValues", type = "variadic", optional = true, description = "Formatting and additional parameters matching string.format's usage." }
	}
}
