# tes3mobileProjectile

A mobile object for a physical projectile.

This type inherits the following: [tes3mobileObject](../../types/tes3mobileObject)
## Properties

### `animTime`

The time index for the projectile model's animation controllers.

**Returns**:

* `result` (number)

***

### `attackSwing`

The attack swing value of the attacker at the time of releasing the projectile.

**Returns**:

* `result` (number)

***

### `boundSize`

A vector that shows the size of the bounding box in each direction.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `cellX`

The X grid coordinate of the cell the mobile is in.

**Returns**:

* `result` (number)

***

### `cellY`

The Y grid coordinate of the cell the mobile is in.

**Returns**:

* `result` (number)

***

### `damage`

Access to the raw damage that this projectile will inflict.

**Returns**:

* `result` (number)

***

### `expire`

A flag that causes the projectile to expire when set to 1.

**Returns**:

* `result` (number)

***

### `firingMobile`

The mobile that fired this projectile.

**Returns**:

* `result` ([tes3mobileActor](../../types/tes3mobileActor))

***

### `firingWeapon`

The weapon that fired this projectile.

**Returns**:

* `result` ([tes3weapon](../../types/tes3weapon))

***

### `flags`

Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

**Returns**:

* `result` (number)

***

### `height`

The height of the mobile above the ground.

**Returns**:

* `result` (number)

***

### `impulseVelocity`

A vector that represents the 3D acceleration of the object.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `initialSpeed`

The initial speed that the projectile was launched at.

**Returns**:

* `result` (number)

***

### `movementCollision`

Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

**Returns**:

* `result` (boolean)

***

### `movementFlags`

Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

**Returns**:

* `result` (number)

***

### `objectType`

The type of mobile object. Maps to values in tes3.objectType.

**Returns**:

* `result` (number)

***

### `playerDistance`

The distance to the player. Updated every frame when the mobile is in an active cell.

**Returns**:

* `result` (number)

***

### `position`

A vector that represents the 3D position of the object.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `prevMovementFlags`

Access to the root mobile object movement flags from the previous frame, represented as an integer. Should not be accessed directly.

**Returns**:

* `result` (number)

***

### `reference`

Access to the reference object for the mobile, if any.

**Returns**:

* `result` ([tes3reference](../../types/tes3reference))

***

### `velocity`

Access to the current velocity of the projectile.

**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***
