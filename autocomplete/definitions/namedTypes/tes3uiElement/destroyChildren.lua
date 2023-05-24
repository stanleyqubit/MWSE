return {
	type = "method",
	description = [[Deletes all the child elements of this element. If any element is bound to text input by `tes3ui.acquireTextInput`_, the input is automatically released.

Some widgets like ScrollPanes are built of multiple layers of elements. When an element is created in a complex widget, it is automatically placed as a child of a content element. When clearing a widget's children, you should use `element:getContentElement():destroyChildren()`.]],
}