#pragma once

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

		/*bool straightCollisionDetector(sf::Vertex vertex1, sf::Vertex vertex2);
		void handleStraightCollision(sf::Vertex vertex1, sf::Vertex vertex2);*/

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
}