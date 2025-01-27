# toml
<div class="search_terms" style="display: none">toml</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

Provides support for interacting with toml data through an extended toml.lua module.

## Functions

### `toml.decode`
<div class="search_terms" style="display: none">decode</div>

Decode string representing toml into a table.

```lua
local result = toml.decode(input)
```

**Parameters**:

* `input` (string): The string to be decoded into a table.

**Returns**:

* `result` (table): The decoded table.

***

### `toml.encode`
<div class="search_terms" style="display: none">encode</div>

Encodes a table into a toml string.

```lua
local result = toml.encode(input)
```

**Parameters**:

* `input` (table): The table to be encoded into toml.

**Returns**:

* `result` (string): The encoded toml.

***

### `toml.loadFile`
<div class="search_terms" style="display: none">loadfile</div>

Loads a toml file from `path`, using `toml.decode`. Unlike the related `json.loadfile` function, this is relative to the current working directory, and not relative to Data Files\MWSE. A file extension should be part of the `path` string.

```lua
local data, error = toml.loadFile(path)
```

**Parameters**:

* `path` (string): The file to write to, relative to the current working directory (typically the Morrowind installation folder).

**Returns**:

* `data` (table?): The decoded data, or `nil` if the file could not be found.
* `error` (table?): Information about why the toml file could not be decoded. This result will only be given if the operation fails.

***

### `toml.saveFile`
<div class="search_terms" style="display: none">savefile</div>

Saves a serializable table to `path`, using `toml.encode`. Unlike the related `json.savefile` function, this is relative to the current working directory, and not relative to Data Files\MWSE. A file extension should be part of the `path` string.

```lua
toml.saveFile(path, data)
```

**Parameters**:

* `path` (string): The file to write to, relative to the current working directory (typically the Morrowind installation folder).
* `data` (table): The data to encode and write.

