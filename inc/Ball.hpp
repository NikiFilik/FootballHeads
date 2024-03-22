#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include "Player.hpp"

namespace nf {
	class Ball {
	private:
		float mPositionX, mPositionY;
		float mSpeedX = 0, mSpeedY = 0;
		float mAccelerationX = 0.f, mAccelerationY = 1200.f;
		float mRadius = 25.f, mDensity = 1.f;
		float mBounceCoefficient = 0.7f;
		sf::Texture mTexture;
		sf::Sprite mSprite;
	public:
		Ball() = default;

		void setup(float positionX, float positionY, std::string fileName);
		void restart(float positionX, float positionY);

		float getPositionX();
		float getPositionY();
		float getRadius();
		sf::Sprite getSprite();

		void update(sf::Time deltaTime, int fieldWidth, int fieldHeight);

		bool leftStraightCollisionDetector(int width);
		bool rightStraightCollisionDetector(int width);
		bool upStraightCollisionDetector(int height);
		bool downStraightCollisionDetector(int height);

		void solveLeftStraightCollision();
		void solveRightStraightCollision();
		void solveUpStraightCollision();
		void solveDownStraightCollision();

		bool circleCollisionDetector(Player player);
		void solveCircleCollision(Player& player);

		bool legCollisionDetector(Player player);
		void solveLegCollision(Player& player);

		bool circleCollisionDetector(sf::CircleShape circle);
		void solveCircleCollision(sf::CircleShape& circle);
	};
}