# projectileHitObject

The projectileHitObject event fires when a projectile collides with an object.

```lua
--- @param e projectileHitObjectEventData
local function projectileHitObjectCallback(e)
end
event.register("projectileHitObject", projectileHitObjectCallback)
```

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `target` ([tes3reference](../../types/tes3reference)): Reference to the object that was hit.
* `firingWeapon` ([tes3weapon](../../types/tes3weapon)): The weapon that fired the projectile.
* `collisionPoint` ([tes3vector3](../../types/tes3vector3)): The collision point of the mobile projectile.
* `mobile` ([tes3mobileProjectile](../../types/tes3mobileProjectile)): The mobile projectile that is expiring.
* `firingReference` ([tes3reference](../../types/tes3reference)): Reference to the actor that fired the projectile.
