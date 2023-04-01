return {
	type = "value",
	description = [[This value indicates whether the NPC is speaking or not. The table below describes the relationship.

Value      | Behavior
---------- | ---------
      -1   | The NPC isn't speaking.
 0.0 - 1.0 | The NPC is speaking. The `lipsyncLevel` can be `0.0` when there is a silent part in the sound file the NPC is currently speaking.
]],
	valuetype = "number",
	examples = {
		["isSpeaking"] = {
			description = "Check if the NPC is speaking",
		}
	}
}
