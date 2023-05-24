return {
	type = "value",
	description = [[Changes how the up vector of the local coordinate frame is calculated. When disabled, it will align towards the +Z axis. When enabled, it will use the local path derivatives with smoothing controlled by `smoothing`. This allows the object to turn upside down, but orientation changes at keyframes may be jerky.]],
	valuetype = "boolean",
}
