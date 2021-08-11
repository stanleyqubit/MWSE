--- @meta

--- This event is triggered when any spell is cast successfully; this includes spells cast via scripts. This occurs at the end of the casting animation, just after the magic projectile has been constructed.
--- @class spellCastedEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field target tes3reference The target of the spell. For self-targeted spells, this matches caster.
--- @field source tes3spell The magic source.
--- @field expGainSchool number Of all the magic effects in the spell, there is a magic school which the caster has the lowest skill at casting. This school determines which skill will gain experience on a successful cast. This school can be altered, or set to nil to remove experience gain, possibly in favour of your own experience calculation.
--- @field caster tes3reference The caster of the spell.
--- @field sourceInstance tes3magicSourceInstance The unique instance of the magic source.