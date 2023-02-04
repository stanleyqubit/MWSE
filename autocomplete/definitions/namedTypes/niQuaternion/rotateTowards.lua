return {
	type = "method",
	description = [[Calculates a spherical linear interpolation between this quaternion and another, limited to a maximum rotation angle. Chooses the shortest path of interpolation between quaternions, which means it minimizes spin but the interpolation arc is limited to pi radians or 180 degrees of interpolation.]],
	arguments = {
		{ name = "target", type = "niQuaternion", description = "The quaternion to interpolate towards." },
		{ name = "rotationLimit", type = "number", description = "The interpolation result will be limited to this maximum angle from the initial quaternion. Angle in radians." },
	},
	returns = {
		{ name = "result", type = "niQuaternion", description = "The calculated result." },
	},
}
