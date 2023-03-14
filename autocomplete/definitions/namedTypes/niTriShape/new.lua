return {
	type = "function",
	description = [[Creates a new niTriShape.]],
	arguments = {
		{ name = "vertexCount", type = "number", description = "The number of vertices the shape will allocate." },
		{ name = "hasNormals", type = "boolean", description = "If `true`, normals will be allocated." },
		{ name = "hasTextureCoords", type = "boolean", description = "If `true`, texture coordinates will be allocated." },
		{ name = "triangleCount", type = "number", description = "The number of triangles to allocate." },
	},
	returns = {
		{ name = "shape", type = "niTriShape" },
	},
}
