return {
	type = "method",
	description = [[Returns true if the messages of the given log level will be logged.]],
	arguments = {
		{ name = "logLevel", type = "mwseLoggerLogLevel", description = [[Options are: "TRACE", "DEBUG", "INFO", "WARN", "ERROR" and "NONE".]] },
	},
	returns = {
		{ name = "doLog", type = "boolean" }
	}
}
