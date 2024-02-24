#pragma once

#include <SFML/Graphics.hpp>

namespace nf {
	class Player {
	private:
		float positionX, positionY;
		float velocityX, velocityY;
		float accelerationX, accelerationY;
		float radius;
		int redColor, greenColor, blueColor;
		sf::CircleShape shape;
	public:
		Player() = default;

		void setup(float positionX, float positionY, float radius, int redColor, int greenColor, int blueColor);

		void setVelocityX(float velocityX);
		void setVelocityY(float velocityY);

		float getPositionX();
		float getPositionY();
		float getVelocityX();
		float getVelocityY();
		float getRadius();
		sf::CircleShape getShape();

		void leftButtonPressed();
		void rightButtonPressed();
		void jumpButtonPressed();

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