--- @meta

--- The combatStop event occurs when an actor is about to leave combat. This event allows scripts to prevent combat from stopping.
--- @class combatStopEventData
--- @field block boolean If set to `true`, vanilla logic will be suppressed. Returning `false` will set this to `true`.
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field actor tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer The mobile actor who is exiting combat.