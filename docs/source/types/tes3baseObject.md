# tes3baseObject

Almost anything that can be represented in the Construction Set is based on this structure.

## Properties

### `blocked`

The blocked state of the object.

**Returns**:

* `result` (boolean)

***

### `deleted`

The deleted state of the object.

**Returns**:

* `result` (boolean)

***

### `disabled`

The disabled state of the object.

**Returns**:

* `result` (boolean)

***

### `id`

The unique identifier for the object.

**Returns**:

* `result` (string)

***

### `modified`

The modification state of the object since the last save.

**Returns**:

* `result` (boolean)

***

### `objectFlags`

The raw flags of the object.

**Returns**:

* `result` (number)

***

### `objectType`

The type of object. Maps to values in tes3.objectType.

**Returns**:

* `result` (number)

***

### `persistent`

The persistent flag of the object.

**Returns**:

* `result` (boolean)

***

### `sourceless`

The soruceless flag of the object.

**Returns**:

* `result` (boolean)

***

### `sourceMod`

The filename of the mod that owns this object.

**Returns**:

* `result` (string)

***

### `supportsLuaData`

If true, references of this object can store temporary or persistent lua data.

**Returns**:

* `result` (boolean)

***

## Methods

### `__tojson`

Serializes the object to json.

```lua
local string = tes3baseObject:__tojson()
```

**Returns**:

* `string` (string)

***
