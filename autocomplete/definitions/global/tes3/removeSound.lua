return {
	type = "function",
	description = [[Stops a sound playing. Without a reference, it will match unattached sounds. With a reference, it will only match a sound playing on that specific reference. To stop a voiceover, pass the wanted actor as the `reference` parameter and `sound = nil`.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "sound", type = "tes3sound|string|nil", description = "The sound object, or id of the sound to look for. If no sound is passed, removes every sound on the reference." },
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The reference the sound is attached to." },
		},
	}},
}
