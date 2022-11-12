<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

# niCollisionGroupRecord

!!! warning
	This part of the API isn't fully understood yet and thus is considered experimental. That means that there can be breaking changes requiring the code using this part of the API to be rewritten. The MWSE team will not make any effort to keep backward compatibility with the mods using experimental APIs.
 This class is used to store the `niAVObject` derived object for collision detection in the parent `niCollisionGroup` collider and collidee arrays.

## Properties

### `collisionRoot`

*Read-only*. This is the ancestor root node added to the parent niCollisionGroup, whose child is the related `.object`.

**Returns**:

* `result` ([niAVObject](../../types/niAVObject))

***

### `object`

*Read-only*. Access to the actual scene graph object this `niCollisionGroupRecord` represents.

**Returns**:

* `result` ([niAVObject](../../types/niAVObject))

***
