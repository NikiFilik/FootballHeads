#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace nf {
	class Player {
	private:
		float mPositionX, mPositionY;
		float mSpeedX = 0.f, mSpeedY = 0.f;
		float mMaxSpeed = 500.f, mBoost = 3000.f, mSlowdown = 1500.f;
		float mJumpForce = 550.f;
		float mAccelerationX = 0.f, mAccelerationY = 1200.f;
		float mRadius = 50.f, mDensity = 1.f;
		float mBounceCoefficient = 0.f;
		sf::Texture mTexture;
		sf::Sprite mSprite;
		sf::Keyboard::Key mLeftKey, mRightKey, mJumpKey, mKickKey;

		int mSide;
		float mLegPositionX, mLegPositionY;
		float mLegSpeedX = 0.f, mLegSpeedY = 0.f;
		float mLegRotationSpeed = 0.f;
		float mLegMaxRotationSpeed = 1200.f, mLegBoost = 3600.f, mLegSlowdown = 1200.f;
		float mLegRadius = 20.f;
		sf::Texture mLegTexture;
		sf::Sprite mLegSprite;

		void legUpdate();
	public:
		Player() = default;

		void setup(float positionX, float positionY, std::string fileName, std::string legFileName, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key jumpKey, sf::Keyboard::Key kickKey, int side);
		void restart(float positionX, float positionY);

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
		sf::Keyboard::Key getKickKey();
		sf::Sprite getLegSprite();
		float getLegPositionX();
		float getLegPositionY();
		float getLegSpeedX();
		float getLegSpeedY();
		float getLegRadius();

		void moveLeft(sf::Time deltaTime);
		void moveRight(sf::Time deltaTime);
		void doKick(sf::Time deltaTime);
		void doJump();

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

		bool circleCollisionDetector(sf::CircleShape circle);
		void solveCircleCollision(sf::CircleShape& circle);
	};
}