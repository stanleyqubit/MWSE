return {
	type = "value",
	description = [[Access to the timer that manages for this callback function. Can be used to cancel the timer inside the callback function or to access the data table stored on the timer.]],
	readOnly = true,
	valuetype = "mwseTimer",
	examples = {
		["..\\..\\..\\global\\timer\\register\\dayLater"] = {
			title = "Persistent timer",
			description = "Examine how the data table stored on timer can be accessed from the callback function.",
		}
	}
}
