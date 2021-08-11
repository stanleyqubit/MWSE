--- @meta

--- This event fires when a dialogue hyperlink is being resolved to a topic, during UI layout (not on link activation).
--- @class infoLinkResolveEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field topic string The topic that activates when this hyperlink is clicked. Initially set to the link text. Set this value to change the target topic.