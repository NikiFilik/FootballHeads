#include "Ball.hpp"

#include <cmath>
#include "functions.hpp"

namespace nf {
	void Ball::setup(float positionX, float positionY, std::string fileName) {
		mPositionX = positionX;
		mPositionY = positionY;
		mSpeedX = nf::randIntFromRange(-400, 401);
		mSpeedY = nf::randIntFromRange(-400, 401);
		mTexture.loadFromFile(fileName);
		mSprite.setTexture(mTexture);
		mSprite.setOrigin(mRadius, mRadius);
		mSprite.setPosition(mPositionX, mPositionY);
	}

	sf::Sprite Ball::getSprite() {
		return mSprite;
	}

	void Ball::update(sf::Time deltaTime, int fieldWidth, int fieldHeight) {
		//исправить врем€
		mPositionX += mSpeedX * deltaTime.asSeconds() + (mAccelerationX * power(deltaTime.asSeconds(), 2)) / 2;
		mPositionY += mSpeedY * deltaTime.asSeconds() + (mAccelerationY * power(deltaTime.asSeconds(), 2)) / 2;

		if (mPositionX - mRadius < 0) {
			mPositionX = mRadius;
		}
		if (mPositionX + mRadius > fieldWidth) {
			mPositionX = fieldWidth - mRadius;
		}
		if (mPositionY - mRadius < 0) {
			mPositionY = mRadius;
		}
		if (mPositionY + mRadius > fieldHeight) {
			mPositionY = fieldHeight - mRadius;
		}

		mSprite.setPosition(mPositionX, mPositionY);
		mSprite.rotate(mSpeedX * deltaTime.asSeconds() / (mRadius * 6.28 / 360));

		mSpeedX += mAccelerationX * deltaTime.asSeconds();
		mSpeedY += mAccelerationY * deltaTime.asSeconds();
	}

	//ƒобавить аргументы дл€ всех пр€мых
	bool Ball::leftStraightCollisionDetector() {
		if (mPositionX <= mRadius && mSpeedX <= 0) {
			return true;
		}
		return false;
	}
	bool Ball::rightStraightCollisionDetector(int width) {
		if (mPositionX >= width - mRadius && mSpeedX >= 0) {
			return true;
		}
		return false;
	}
	bool Ball::upStraightCollisionDetector() {
		if (mPositionY <= mRadius && mSpeedY <= 0) {
			return true;
		}
		return false;
	}
	bool Ball::downStraightCollisionDetector(int height) {
		if (mPositionY >= height - mRadius && mSpeedY >= 0) {
			return true;
		}
		return false;
	}

	void Ball::solveLeftStraightCollision() {
		mSpeedX *= -mBounceCoefficient;
	}
	void Ball::solveRightStraightCollision() {
		mSpeedX *= -mBounceCoefficient;
	}
	void Ball::solveUpStraightCollision() {
		mSpeedY *= -mBounceCoefficient;
	}
	void Ball::solveDownStraightCollision() {
		mSpeedY *= -mBounceCoefficient;
	}

	bool Ball::circleCollisionDetector(Player player) {
		if (power(mPositionX - player.getPositionX(), 2) + power(mPositionY - player.getPositionY(), 2) <= power(mRadius + player.getRadius(), 2)) {
			float deltaPositionX = mPositionX - player.getPositionX();
			float deltaPositionY = mPositionY - player.getPositionY();
			float deltaSpeedX = mSpeedX - player.getSpeedX();
			float deltaSpeedY = mSpeedY - player.getSpeedY();
			if (deltaPositionX * deltaSpeedX + deltaPositionY * deltaSpeedY <= 0) {
				return true;
			}
		}
		return false;
	}

	void Ball::solveCircleCollision(Player& player) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float unitNormalVectorX, unitNormalVectorY, unitTangentVectorX, unitTangentVectorY;
		nf::findCoordinatesOfUnitNormalVector(mPositionX, mPositionY, player.getPositionX(), player.getPositionY(), unitNormalVectorX, unitNormalVectorY);
		unitTangentVectorX = unitNormalVectorY;
		unitTangentVectorY = -unitNormalVectorX;

		float ballNormalSpeed = unitNormalVectorX * mSpeedX + unitNormalVectorY * mSpeedY;
		float ballTangentSpeed = unitTangentVectorX * mSpeedX + unitTangentVectorY * mSpeedY;
		float playerNormalSpeed = unitNormalVectorX * player.getSpeedX() + unitNormalVectorY * player.getSpeedY();
		float playerTangentSpeed = unitTangentVectorX * player.getSpeedX() + unitTangentVectorY * player.getSpeedY();

		float ballMass = power(mRadius, 3) * mDensity, playerMass = power(player.getRadius(), 3) * player.getDensity();
		float newBallNormalSpeed, newBallTangentSpeed, newPlayerNormalSpeed, newPlayerTangentSpeed;
		nf::solveOneDimensionalCollision(ballMass, playerMass, ballNormalSpeed, playerNormalSpeed, newBallNormalSpeed, newPlayerNormalSpeed);
		newBallTangentSpeed = ballTangentSpeed;
		newPlayerTangentSpeed = playerTangentSpeed;

		float newBallNormalSpeedX = newBallNormalSpeed * unitNormalVectorX, newBallNormalSpeedY = newBallNormalSpeed * unitNormalVectorY;
		float newBallTangentSpeedX = newBallTangentSpeed * unitTangentVectorX, newBallTangentSpeedY = newBallTangentSpeed * unitTangentVectorY;
		float newPlayerNormalSpeedX = newPlayerNormalSpeed * unitNormalVectorX, newPlayerNormalSpeedY = newPlayerNormalSpeed * unitNormalVectorY;
		float newPlayerTangentSpeedX = newPlayerTangentSpeed * unitTangentVectorX, newPlayerTangentSpeedY = newPlayerTangentSpeed * unitTangentVectorY;

		mSpeedX = newBallNormalSpeedX + newBallTangentSpeedX;
		mSpeedY = newBallNormalSpeedY + newBallTangentSpeedY;
		player.setSpeedX(newPlayerNormalSpeedX + newPlayerTangentSpeedX);
		player.setSpeedY(newPlayerNormalSpeedY + newPlayerTangentSpeedY);
	}
}