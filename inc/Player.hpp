#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace nf {
	class Player {
	private:
		float mPositionX, mPositionY;
		float mSpeedX = 0.f, mSpeedY = 0.f;
		float mMaxSpeed = 500.f, mBoost = 2000.f, mSlowdown = 1000.f;
		float mJumpForce = 700.f;
		float mAccelerationX = 0.f, mAccelerationY = 1600.f;
		float mRadius = 50.f, mDensity = 1.f;
		float mBounceCoefficient = 0.f;
		sf::Texture mTexture;
		sf::Sprite mSprite;
		sf::Keyboard::Key mLeftKey, mRightKey, mJumpKey;
	public:
		Player() = default;

		void setup(float positionX, float positionY, std::string fileName, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key jumpKey);

		void setSpeedX(float speed);
		void setSpeedY(float speed);

		float getPositionX();
		float getPositionY();
		float getSpeedX();
		float getSpeedY();
		float getRadius();
		float getDensity();
		sf::Sprite getSprite();
		sf::Keyboard::Key getLeftKey();
		sf::Keyboard::Key getRightKey();
		sf::Keyboard::Key getJumpKey();

		void moveLeft(sf::Time deltaTime);
		void moveRight(sf::Time deltaTime);
		void doJump();

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