#pragma once

#include <SFML/Graphics.hpp>

#include "Player.hpp"

namespace nf {
	class Ball {
	private:
		float mPositionX, mPositionY;
		float mSpeedX, mSpeedY;
		float mMaxSpeed = 1000;
		float mAccelerationX = 0, mAccelerationY = 1500;
		float mJumpingCoefficient = 0.7;
		float mRadius = 25;
		sf::Texture mTexture;
		sf::Sprite mSprite;
	public:
		void setup(float positionX, float positionY);

		sf::Sprite getSprite();

		void move(sf::Time deltaTime, int width, int height);

		void handleWallsCollision(int width, int height);

		/*bool straightCollisionDetector(sf::Vertex vertex1, sf::Vertex vertex2);
		void handleStraightCollision(sf::Vertex vertex1, sf::Vertex vertex2);*/

		bool circleCollisionDetector(Player player);
		void handleCircleCollision(Player& player);
	};
}