return {
	type = "value",
	description = [[Equal to the `APPVEYOR_BUILD_NUMBER` in builds by AppVeyor (these builds are installed using the MWSE-Update). Equal to `UINT_MAX` in regular builds. This number is used for [mod metadata](https://mwse.github.io/MWSE/guides/metadata/#dependencies-section) files, when a mod depends on MWSE being installed.]],
	valuetype = "integer",
}