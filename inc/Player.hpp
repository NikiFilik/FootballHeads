#pragma once

#include <SFML/Graphics.hpp>

#include <string>

namespace nf {
	class Player {
	private:
		float mPositionX, mPositionY;
		float mSpeedX = 0.f, mSpeedY = 0.f;
		float mMaxSpeed = 600.f, mBoost = 100.f, mSlowdown = 50.f;
		float mJumpForce = 500.f;
		float mAccelerationX = 0.f, mAccelerationY = 1000.f;
		float mRadius = 50.f, mDensity = 1.f;
		float mBounceCoefficient = 0.1f;
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

		bool leftWallCollisionDetector();
		bool rightWallCollisionDetector(int fieldWidth);
		bool upWallCollisionDetector();
		bool downWallCollisionDetector(int fieldHeight);

		void solveLeftWallCollision();
		void solveRightWallCollision();
		void solveUpWallCollision();
		void solveDownWallCollision();

		bool circleCollisionDetector(Player other);
		void solveCircleCollision(Player& other);
	};
}

/*#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace nf {
	class Player {
	private:
		float mPositionX, mPositionY;
		float mSpeedX = 0, mSpeedY = 0;
		float mMaxSpeedX = 150;
		float mAccelerationY = 1500;
		float mAccelerationBoost = 200, mAccelerationBreak = 1000;
		float mSlipCoefficient = 1, mJumpingCoefficient = 0.2;
		float mRadius = 50;

		sf::Texture mTexture;
		sf::Sprite mSprite;

		sf::Keyboard::Key mLeftKey, mRightKey, mJumpKey;
	public:
		void setup(float positionX, float positionY, std::string fileName, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key jump);

		sf::Sprite getSprite();

		//void update(sf::Vertex vertex1, sf::Vertex vertex2, nf::Player otherPlayer);

		void move(sf::Time deltaTime, int width, int height);

		void moveLeft(sf::Time deltaTime);
		void moveRight(sf::Time deltaTime);
		void doJump();

		void handleWallsCollision(int width, int height);

		bool straightCollisionDetector(sf::Vertex vertex1, sf::Vertex vertex2);
		void handleStraightCollision(sf::Vertex vertex1, sf::Vertex vertex2);

		bool circleCollisionDetector(Player player);
		void handleCircleCollision(Player& player);

		float getPositionX();
		float getPositionY();
		float getSpeedX();
		float getSpeedY();
		float getRadius();

		sf::Keyboard::Key getLeftKey();
		sf::Keyboard::Key getRightKey();
		sf::Keyboard::Key getJumpKey();

		void setSpeedX(float speed);
		void setSpeedY(float speed);
	};
}*/