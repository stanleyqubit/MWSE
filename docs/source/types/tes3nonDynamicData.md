# tes3nonDynamicData

A child container from tes3dataHandler, where game data is stored.

## Properties

### `birthsigns`

A collection of all birthsign objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `cells`

A collection of all cell objects.

**Returns**:

* `result` (tes3stlList)

***

### `classes`

A collection of all class objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `dialogues`

A collection of all dialogue objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `factions`

A collection of all faction objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `globals`

A collection of all global variable objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `magicEffects`

A table of references to all 143 magic effects.

**Returns**:

* `result` (table)

***

### `objects`

A collection of all other game objects.

**Returns**:

* `result` (tes3linkedList)

***

### `races`

A collection of all race objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `regions`

A collection of all region objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `scripts`

A collection of all script objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `skills`

A table of references to all 27 skills.

**Returns**:

* `result` (table)

***

### `soundGenerators`

A collection of all sound generator objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `sounds`

A collection of all sound objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

### `spells`

A collection of all spell objects.

**Returns**:

* `result` (tes3linkedList)

***

### `startScripts`

A collection of all tes3startScript objects.

**Returns**:

* `result` ([tes3iterator](../../types/tes3iterator))

***

## Methods

### `addNewObject`

Inserts a newly created object into the proper collections.

```lua
local result = tes3nonDynamicData:addNewObject(object)
```

**Parameters**:

* `object` ([tes3baseObject](../../types/tes3baseObject))

**Returns**:

* `result` (boolean)

***

### `deleteObject`

Removes an object from the proper collections.

```lua
tes3nonDynamicData:deleteObject(object)
```

**Parameters**:

* `object` ([tes3baseObject](../../types/tes3baseObject))

***

### `drawCellMapMarker`

Draws a map marker box for an exterior cell onto the world map.

```lua
tes3nonDynamicData:drawCellMapMarker(cell)
```

**Parameters**:

* `cell` ([tes3cell](../../types/tes3cell))

***

### `findDialogue`

Locates a dialogue for a given ID.

```lua
local result = tes3nonDynamicData:findDialogue(id)
```

**Parameters**:

* `id` (string)

**Returns**:

* `result` ([tes3dialogue](../../types/tes3dialogue))

***

### `findFirstCloneOfActor`

Locates the first reference for a given object ID.

```lua
local result = tes3nonDynamicData:findFirstCloneOfActor(id)
```

**Parameters**:

* `id` (string)

**Returns**:

* `result` ([tes3reference](../../types/tes3reference))

***

### `findGlobalVariable`

Locates a global variable for a given ID.

```lua
local result = tes3nonDynamicData:findGlobalVariable(id)
```

**Parameters**:

* `id` (string)

**Returns**:

* `result` ([tes3globalVariable](../../types/tes3globalVariable))

***

### `findScript`

Locates a script for a given ID.

```lua
local result = tes3nonDynamicData:findScript(id)
```

**Parameters**:

* `id` (string)

**Returns**:

* `result` (tes3script)

***

### `findSound`

Locates a sound for a given ID.

```lua
local result = tes3nonDynamicData:findSound(id)
```

**Parameters**:

* `id` (string)

**Returns**:

* `result` ([tes3sound](../../types/tes3sound))

***

### `resolveObject`

Locates a general object for a given ID.

```lua
local result = tes3nonDynamicData:resolveObject(id)
```

**Parameters**:

* `id` (string)

**Returns**:

* `result` ([tes3baseObject](../../types/tes3baseObject))

***

### `showLocationOnMap`

Draws map marker boxes onto the world map for all cells with names starting with the prefix. e.g. showLocationOnMap("Vivec") draws boxes for all cells in Vivec, because the cell names "Vivec, Arena", "Vivec, Temple", etc. matches.

```lua
tes3nonDynamicData:showLocationOnMap(cellNamePrefix)
```

**Parameters**:

* `cellNamePrefix` (string)

***
