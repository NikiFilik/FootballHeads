#pragma once

#include <SFML/Graphics.hpp>

#include <Player.hpp>

namespace nf {
	class Ball {
	private:
		float positionX, positionY;
		float velocityX, velocityY;
		float accelerationX, accelerationY;
		float radius;
		sf::CircleShape shape;
	public:
		Ball() = default;

		void setup(float positionX, float positionY, float radius);

		sf::CircleShape getShape();

		void move(float deltaTime, int width, int height);

		bool leftWallCollisionDetector();
		bool rightWallCollisionDetector(int width);
		bool upWallCollisionDetector();
		bool downWallCollisionDetector(int height);

		void calculateLeftWallCollision();
		void calculateRightWallCollision();
		void calculateUpWallCollision();
		void calculateDownWallCollision();

		bool playerCollisionDetector(Player player);
		void calculatePlayerCollision(Player& player);
	};
}