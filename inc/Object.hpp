#pragma once

#include <SFML/Graphics.hpp>

namespace nf {
	class Object {
	private:
		int mNumberOfVertices;
		sf::Vertex* mVertexArray;
	public:
		Object() = default;
		~Object();

		void setup(int number);

		sf::Vertex& operator[](int index);

		sf::Vertex* getShape();
	};
}