#include "Object.hpp"

namespace nf {
	Object::~Object() {
		delete[] mVertexArray;
	}

	void Object::setup(int number) {
		mNumberOfVertices = number;
		mVertexArray = new sf::Vertex[number];
	}

	sf::Vertex& Object::operator[](int index) {
		return mVertexArray[index];
	}

	sf::Vertex* Object::getShape() {
		return mVertexArray;
	}
}