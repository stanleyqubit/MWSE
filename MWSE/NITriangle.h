#pragma once

namespace NI {
	struct Triangle {
		unsigned short vertices[3];

		Triangle();
		Triangle(unsigned short first, unsigned short second, unsigned short third);
		Triangle(const sol::table table);

		Triangle& operator=(const sol::table table);

		std::reference_wrapper<unsigned short[3]> getVertices();
	};
}
