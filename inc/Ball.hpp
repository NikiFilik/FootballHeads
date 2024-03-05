#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include "Player.hpp"

namespace nf {
	class Ball {
	private:
		float mPositionX, mPositionY;
		float mSpeedX, mSpeedY;
		float mAccelerationX = 0.f, mAccelerationY = 1600.f;
		float mRadius = 25.f, mDensity = 1.f;
		float mBounceCoefficient = 0.7f;
		sf::Texture mTexture;
		sf::Sprite mSprite;
	public:
		Ball() = default;

		void setup(float positionX, float positionY, std::string fileName);

		sf::Sprite getSprite();

		void update(sf::Time deltaTime, int fieldWidth, int fieldHeight);

		bool leftStraightCollisionDetector();
		bool rightStraightCollisionDetector(int width);
		bool upStraightCollisionDetector();
		bool downStraightCollisionDetector(int height);

		void solveLeftStraightCollision();
		void solveRightStraightCollision();
		void solveUpStraightCollision();
		void solveDownStraightCollision();

		bool circleCollisionDetector(Player player);
		void solveCircleCollision(Player& player);
	};
}