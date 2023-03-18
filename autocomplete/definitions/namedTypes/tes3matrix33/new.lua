return {
	type = "function",
	description = [[Creates a new 3 by 3 matrix from 3 provided vectors or 9 numbers. Creates an empty matrix if nothing is provided.]],
	arguments = {
		{ name = "x0", type = "number|tes3vector3", optional = true, default = 0 },
		{ name = "y0", type = "number|tes3vector3", optional = true, default = 0 },
		{ name = "z0", type = "number|tes3vector3", optional = true, default = 0 },
		{ name = "x1", type = "number", optional = true, default = 0 },
		{ name = "y1", type = "number", optional = true, default = 0 },
		{ name = "z1", type = "number", optional = true, default = 0 },
		{ name = "x2", type = "number", optional = true, default = 0 },
		{ name = "y2", type = "number", optional = true, default = 0 },
		{ name = "z2", type = "number", optional = true, default = 0 },
	},
	returns = {{ name = "matrix", type = "tes3matrix33" }},
}