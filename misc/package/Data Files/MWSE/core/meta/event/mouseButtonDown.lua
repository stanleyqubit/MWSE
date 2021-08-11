--- @meta

--- The mouseButtonDown event fires when a button on the mouse is pressed.
--- @class mouseButtonDownEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field isSuperDown number True if super (Windows key) is held.
--- @field button number The button index that was pressed.
--- @field isControlDown number True if control is held.
--- @field isAltDown number True if alt  is held.