#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include "Player.hpp"

namespace nf {
	class Ball {
	private:
		float mPositionX, mPositionY;
		float mSpeedX, mSpeedY;
		float mAccelerationX = 0.f, mAccelerationY = 1000.f;
		float mRadius = 25.f, mDensity = 1.f;
		float mBounceCoefficient = 0.7f;
		sf::Texture mTexture;
		sf::Sprite mSprite;
	public:
		Ball() = default;

		void setup(float positionX, float positionY, std::string filename);

		sf::Sprite getSprite();

		void update(sf::Time deltaTime, int fieldWidth, int fieldHeight);

		bool leftWallCollisionDetector();
		bool rightWallCollisionDetector(int fieldWidth);
		bool upWallCollisionDetector();
		bool downWallCollisionDetector(int fieldHeight);

		void solveLeftWallCollision();
		void solveRightWallCollision();
		void solveUpWallCollision();
		void solveDownWallCollision();

		bool circleCollisionDetector(Player player);
		void solveCircleCollision(Player& player);
	};
}

/*#pragma once

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

		bool straightCollisionDetector(sf::Vertex vertex1, sf::Vertex vertex2);
		void handleStraightCollision(sf::Vertex vertex1, sf::Vertex vertex2);

		bool circleCollisionDetector(Player player);
		void handleCircleCollision(Player& player);
	};
}*/