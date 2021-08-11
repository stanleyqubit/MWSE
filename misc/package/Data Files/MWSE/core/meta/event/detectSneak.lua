--- @meta

--- This event is invoked whenever an actor is checking if they can detect another actor sneaking.
--- @class detectSneakEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field target tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer The mobile actor that is possibly being detected.
--- @field isDetected boolean The previous detection calculated, starting with the base engine result.
--- @field detector tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer The mobile actor that is doing the detecting.