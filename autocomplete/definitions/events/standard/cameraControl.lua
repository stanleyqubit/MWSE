return {
	type = "event",
	description = "This event is raised when the camera's position or rotation matrix is changed. It can be used to alter the camera's transformations by changing `cameraTransform` for the third-person camera or `armCameraTransform` for the first-person camera.",
	eventData = {
		["animationController"] = {
			type = "tes3playerAnimationController",
			readOnly = true,
			description = "Provides access to the player's animation controller.",
		},
		["previousCameraTransform"] = {
			type = "tes3transform",
			readOnly = true,
			description = "The camera transformation before any changes. Identical to `cameraTransform` if the player teleported a significant distance.",
		},
		["previousArmCameraTransform"] = {
			type = "tes3transform",
			readOnly = true,
			description = "The arm camera transformation before any changes. Identical to `armCameraTransform` if the player teleported a significant distance.",
		},
		["cameraTransform"] = {
			type = "tes3transform",
			description = "The new camera transformation.",
		},
		["armCameraTransform"] = {
			type = "tes3transform",
			description = "The new arm camera transformation.",
		},
	},
}