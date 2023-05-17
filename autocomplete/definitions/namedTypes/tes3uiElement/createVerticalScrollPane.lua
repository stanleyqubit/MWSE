return {
	type = "method",
	description = [[Creates a vertically scrolling pane. Useful as a list box.

Scroll panes create a complex UI subtree, with a container for child elements. create* methods automatically place new elements in this container, not as a direct child of the scroll pane. The container element can be accessed with the `getContentElement()` method. It should be used when iterating or clearing the scroll pane contents.

Scroll pane specific properties can be accessed through the `widget` property. The widget type for scroll panes is [`tes3uiScrollPane`](https://mwse.github.io/MWSE/types/tes3uiScrollPane/).]],
	arguments = { {
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "id", type = "string|number", description = "An identifier to help find this element later.", optional = true },
		},
	} },
	valuetype = "tes3uiElement",
	examples = {
		["sorting"] = {
			title = "Sorting the child elements in a scroll pane"
		}
	}
}
