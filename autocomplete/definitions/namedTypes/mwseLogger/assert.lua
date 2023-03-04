return {
	type = "method",
	description = [[Assert a condition and log an error if it fails.]],
	arguments = {
		{ name = "condition", type = "boolean" },
		{ name = "message", type = "string" },
		{ name = "formatValues", type = "variadic", optional = true, description = "Formatting and additional parameters matching string.format's usage." }
	}
}
