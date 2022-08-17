<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

# niParticleCollider

This modifier provides framework for collision testing. The classes derived from this one: niSphericalCollider, and niPlanarCollider, test for collisions. Each frame a niParticleSystemController traverses its list of niParticlesColliders to determine if any collisions happened.

This type inherits the following: [niParticleModifier](../../types/niParticleModifier), [niObject](../../types/niObject)
## Properties

### `collisionPoint`



**Returns**:

* `result` ([tes3vector3](../../types/tes3vector3))

***

### `collisionTime`



**Returns**:

* `result` (number)

***

### `controller`

Access to the generic time controller for particle systems.

**Returns**:

* `result` ([niParticleSystemController](../../types/niParticleSystemController))

***

### `dieOnCollide`

If this flag is true a particle that collides with something will be destroyed.

**Returns**:

* `result` (boolean)

***

### `next`

Access to the next attached modifier.

**Returns**:

* `result` ([niParticleModifier](../../types/niParticleModifier))

***

### `refCount`

*Read-only*. The number of references that exist for this object. When this value reaches zero, the object will be deleted.

**Returns**:

* `result` (number)

***

### `restitution`

The fraction of the original amount of energy the particle retains after a collision.

**Returns**:

* `result` (number)

***

### `RTTI`

*Read-only*. The runtime type information for this object. This is an alias for the `.runTimeTypeInformation` property.

**Returns**:

* `result` ([niRTTI](../../types/niRTTI))

***

### `runTimeTypeInformation`

*Read-only*. The runtime type information for this object.

**Returns**:

* `result` ([niRTTI](../../types/niRTTI))

***

### `spawnOnCollide`

This flag controls if a new particle will be spawned on a collision. The new particle is spawned at the position of the old particle with the same attributes.

**Returns**:

* `result` (boolean)

***

## Methods

### `clone`

Creates a copy of this object.

```lua
local result = myObject:clone()
```

**Returns**:

* `result` ([niObject](../../types/niObject))

***

### `isInstanceOfType`

Determines if the object is of a given type, or of a type derived from the given type. Types can be found in the [`tes3.niType`](https://mwse.github.io/MWSE/references/niTypes/) table.

```lua
local result = myObject:isInstanceOfType(type)
```

**Parameters**:

* `type` (number): Use values in the [`tes3.niType`](https://mwse.github.io/MWSE/references/niTypes/) table.

**Returns**:

* `result` (boolean)

***

### `isOfType`

Determines if the object is of a given type. Types can be found in the [`tes3.niType`](https://mwse.github.io/MWSE/references/niTypes/) table.

```lua
local result = myObject:isOfType(type)
```

**Parameters**:

* `type` (number): Use values in the [`tes3.niType`](https://mwse.github.io/MWSE/references/niTypes/) table.

**Returns**:

* `result` (boolean)

***

### `saveBinary`

Serializes the object, and writes it to the given file.

```lua
local success = myObject:saveBinary(path)
```

**Parameters**:

* `path` (string): The path to write the file at, relative to the Morrowind installation folder.

**Returns**:

* `success` (boolean): If true the object was successfully serialized.

***
