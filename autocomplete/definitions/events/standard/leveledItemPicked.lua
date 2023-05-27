return {
	type = "event",
	description = "This event is called after any leveled item list is resolved.",
	related = { "leveledCreaturePicked", "leveledItemPicked" },
	eventData = {
		["list"] = {
			type = "tes3leveledItem",
			readOnly = true,
			description = "The list responsible for creating a pick.",
		},
		["pick"] = {
			type = "tes3item",
			--readOnly = true,
			description = "The result of the pick by vanilla mechanism.", -- This can be modified to change the result to something else.
		},
		["spawner"] = {
			type = "tes3reference|nil",
			--readOnly = true,
			description = "The reference for the inventory that the item is being picked in. In a few cases this may be `nil`. Note that during actor cloning, the inventory is resolved before cloning is fully complete.",
		},
	},
	filter = "list",
	blockable = true,
}
