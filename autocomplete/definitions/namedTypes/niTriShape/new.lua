return {
	type = "function",
	description = [[Creates a new niTriShape.]],
	arguments = {
		{ name = "vertexCount", type = "number", description = "The number of vertices the shape will allocate." },
		{ name = "hasNormals", type = "boolean", description = "If `true`, normals will be allocated." },
		{ name = "hasColors", type = "boolean", description = "If `true`, colors will be allocated." },
		{ name = "textureCoordSets", type = "number", description = "The number of texture coordinate sets (UV sets) to allocate." },
		{ name = "triangleCount", type = "number", description = "The number of triangles to allocate." },
	},
	returns = {
		{ name = "shape", type = "niTriShape" },
	},
}
