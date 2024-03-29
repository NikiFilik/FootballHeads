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
	void Ball::restart(float positionX, float positionY) {
		mPositionX = positionX;
		mPositionY = positionY;
		mSpeedX = nf::randIntFromRange(-400, 401);
		mSpeedY = nf::randIntFromRange(-400, 401);
	}

	float Ball::getPositionX() {
		return mPositionX;
	}
	float Ball::getPositionY() {
		return mPositionY;
	}
	float Ball::getRadius() {
		return mRadius;
	}
	sf::Sprite Ball::getSprite() {
		return mSprite;
	}

	void Ball::update(sf::Time deltaTime, int fieldWidth, int fieldHeight) {

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

	bool Ball::leftStraightCollisionDetector(int width) {
		if (mPositionX <= width + mRadius && mSpeedX <= 0) {
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
	bool Ball::upStraightCollisionDetector(int height) {
		if (mPositionY <= height + mRadius && mSpeedY <= 0 && mPositionY > height) {
			return true;
		}
		return false;
	}
	bool Ball::downStraightCollisionDetector(int height) {
		if (mPositionY >= height - mRadius && mSpeedY >= 0 && mPositionY < height) {
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

	bool Ball::legCollisionDetector(Player player) {
		if (power(mPositionX - player.getLegPositionX(), 2) + power(mPositionY - player.getLegPositionY(), 2) <= power(mRadius + player.getLegRadius(), 2)) {
			float deltaPositionX = mPositionX - player.getLegPositionX();
			float deltaPositionY = mPositionY - player.getLegPositionY();
			float deltaSpeedX = mSpeedX - player.getLegSpeedX();
			float deltaSpeedY = mSpeedY - player.getLegSpeedY();
			if (deltaPositionX * deltaSpeedX + deltaPositionY * deltaSpeedY <= 0) {
				return true;
			}
		}
		return false;
	}

	void Ball::solveLegCollision(Player& player) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float unitNormalVectorX, unitNormalVectorY, unitTangentVectorX, unitTangentVectorY;
		nf::findCoordinatesOfUnitNormalVector(mPositionX, mPositionY, player.getLegPositionX(), player.getLegPositionY(), unitNormalVectorX, unitNormalVectorY);
		unitTangentVectorX = unitNormalVectorY;
		unitTangentVectorY = -unitNormalVectorX;

		float ballNormalSpeed = unitNormalVectorX * mSpeedX + unitNormalVectorY * mSpeedY;
		float ballTangentSpeed = unitTangentVectorX * mSpeedX + unitTangentVectorY * mSpeedY;
		float playerNormalSpeed = unitNormalVectorX * player.getLegSpeedX() + unitNormalVectorY * player.getLegSpeedY();
		float playerTangentSpeed = unitTangentVectorX * player.getLegSpeedX() + unitTangentVectorY * player.getLegSpeedY();

		float ballMass = power(mRadius, 3) * mDensity, playerMass = power(player.getLegRadius(), 3) * player.getDensity() * 10000.f;
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
		//player.setSpeedX(newPlayerNormalSpeedX + newPlayerTangentSpeedX);
		//player.setSpeedY(newPlayerNormalSpeedY + newPlayerTangentSpeedY);
	}

	bool Ball::circleCollisionDetector(sf::CircleShape circle) {
		if (power(mPositionX - circle.getPosition().x, 2) + power(mPositionY - circle.getPosition().y, 2) <= power(mRadius + circle.getRadius(), 2)) {
			float deltaPositionX = mPositionX - circle.getPosition().x;
			float deltaPositionY = mPositionY - circle.getPosition().y;
			float deltaSpeedX = mSpeedX - 0;
			float deltaSpeedY = mSpeedY - 0;
			if (deltaPositionX * deltaSpeedX + deltaPositionY * deltaSpeedY <= 0) {
				return true;
			}
		}
		return false;
	}
	void Ball::solveCircleCollision(sf::CircleShape& circle) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float unitNormalVectorX, unitNormalVectorY, unitTangentVectorX, unitTangentVectorY;
		nf::findCoordinatesOfUnitNormalVector(mPositionX, mPositionY, circle.getPosition().x, circle.getPosition().y, unitNormalVectorX, unitNormalVectorY);
		unitTangentVectorX = unitNormalVectorY;
		unitTangentVectorY = -unitNormalVectorX;

		float ballNormalSpeed = unitNormalVectorX * mSpeedX + unitNormalVectorY * mSpeedY;
		float ballTangentSpeed = unitTangentVectorX * mSpeedX + unitTangentVectorY * mSpeedY;
		float circleNormalSpeed = 0;
		float circleTangentSpeed = 0;

		float ballMass = power(mRadius, 3) * mDensity, circleMass = power(circle.getRadius(), 3) * 1000.f;
		float newBallNormalSpeed, newBallTangentSpeed, newCircleNormalSpeed, newCircleTangentSpeed;
		nf::solveOneDimensionalCollision(ballMass, circleMass, ballNormalSpeed, circleNormalSpeed, newBallNormalSpeed, newCircleNormalSpeed);
		newBallTangentSpeed = ballTangentSpeed;

		float newBallNormalSpeedX = newBallNormalSpeed * unitNormalVectorX, newBallNormalSpeedY = newBallNormalSpeed * unitNormalVectorY;
		float newBallTangentSpeedX = newBallTangentSpeed * unitTangentVectorX, newBallTangentSpeedY = newBallTangentSpeed * unitTangentVectorY;

		mSpeedX = newBallNormalSpeedX + newBallTangentSpeedX;
		mSpeedY = newBallNormalSpeedY + newBallTangentSpeedY;
	}
}