--- @meta

--- This event is called when determining the cost of making a spell. The price can be modified.
--- @class calcSpellmakingPriceEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor of the spellmaking service npc the player is interacting with.
--- @field price number The price of the spell.
--- @field reference tes3reference *Read-only*. Mobile's related reference.