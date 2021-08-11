--- @meta

--- uiEvent is triggered through various UI events. This includes scrolling through panes, clicking buttons, selecting icons, or a host of other UI-related activities.
--- @class uiEventEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field property number The property identifier that is being triggered.
--- @field block tes3uiElement The UI element that is firing this event.
--- @field var2 number One of two undefined variables related to the event.
--- @field parent tes3uiElement The calling element's parent.
--- @field var1 number One of two undefined variables related to the event.