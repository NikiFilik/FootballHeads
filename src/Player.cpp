#include "Player.hpp"
#include "functions.hpp"

#include <cmath>

namespace nf {
	void Player::setup(float positionX, float positionY, std::string fileName, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key jumpKey) {
		mPositionX = positionX;
		mPositionY = positionY;
		mTexture.loadFromFile(fileName);
		mSprite.setTexture(mTexture);
		mSprite.setOrigin(mRadius, mRadius);
		mSprite.setPosition(mPositionX, mPositionY);
		mLeftKey = leftKey;
		mRightKey = rightKey;
		mJumpKey = jumpKey;
	}

	void Player::setSpeedX(float speed) {
		mSpeedX = speed;
	}
	void Player::setSpeedY(float speed) {
		mSpeedY = speed;
	}

	float Player::getPositionX() {
		return mPositionX;
	}
	float Player::getPositionY() {
		return mPositionY;
	}
	float Player::getSpeedX() {
		return mSpeedX;
	}
	float Player::getSpeedY() {
		return mSpeedY;
	}
	float Player::getRadius() {
		return mRadius;
	}
	float Player::getDensity() {
		return mDensity;
	}
	sf::Sprite Player::getSprite() {
		return mSprite;
	}
	sf::Keyboard::Key Player::getLeftKey() {
		return mLeftKey;
	}
	sf::Keyboard::Key Player::getRightKey() {
		return mRightKey;
	}
	sf::Keyboard::Key Player::getJumpKey() {
		return mJumpKey;
	}

	void Player::moveLeft(sf::Time deltaTime) {
		mSpeedX = std::max(-mMaxSpeed, mSpeedX - mBoost * deltaTime.asSeconds());
	}
	void Player::moveRight(sf::Time deltaTime) {
		mSpeedX = std::min(mMaxSpeed, mSpeedX + mBoost * deltaTime.asSeconds());
	}
	void Player::doJump() {
		mSpeedY = -mJumpForce;
	}

	void Player::update(sf::Time deltaTime, int fieldWidth, int fieldHeight) {
		mPositionX += mSpeedX * deltaTime.asSeconds()/* + (mAccelerationX * power(deltaTime.asSeconds(), 2)) / 2*/;
		mPositionY += mSpeedY * deltaTime.asSeconds()/* + (mAccelerationY * power(deltaTime.asSeconds(), 2)) / 2*/;

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

		mSpeedX += mAccelerationX * deltaTime.asSeconds();
		mSpeedY += mAccelerationY * deltaTime.asSeconds();

		if (mSpeedX > 0) {
			mSpeedX = std::max(0.f, mSpeedX - mSlowdown * deltaTime.asSeconds());
		}
		if (mSpeedX < 0) {
			mSpeedX = std::min(0.f, mSpeedX + mSlowdown * deltaTime.asSeconds());
		}
	}

	bool Player::leftStraightCollisionDetector(int width) {
		if (mPositionX <= width + mRadius && mSpeedX <= 0) {
			return true;
		}
		return false;
	}
	bool Player::rightStraightCollisionDetector(int width) {
		if (mPositionX >= width - mRadius && mSpeedX >= 0) {
			return true;
		}
		return false;
	}
	bool Player::upStraightCollisionDetector(int height) {
		if (mPositionY <= height + mRadius && mSpeedY <= 0 && mPositionY > height) {
			return true;
		}
		return false;
	}
	bool Player::downStraightCollisionDetector(int height) {
		if (mPositionY >= height - mRadius && mSpeedY >= 0 && mPositionY < height) {
			return true;
		}
		return false;
	}

	void Player::solveLeftStraightCollision() {
		mSpeedX *= -mBounceCoefficient;
	}
	void Player::solveRightStraightCollision() {
		mSpeedX *= -mBounceCoefficient;
	}
	void Player::solveUpStraightCollision() {
		mSpeedY *= -mBounceCoefficient;
	}
	void Player::solveDownStraightCollision() {
		mSpeedY *= -mBounceCoefficient;
	}

	bool Player::circleCollisionDetector(Player other) {
		if (power(mPositionX - other.getPositionX(), 2) + power(mPositionY - other.getPositionY(), 2) <= power(mRadius + other.getRadius(), 2)) {
			float deltaPositionX = mPositionX - other.getPositionX();
			float deltaPositionY = mPositionY - other.getPositionY();
			float deltaSpeedX = mSpeedX - other.getSpeedX();
			float deltaSpeedY = mSpeedY - other.getSpeedY();
			if (deltaPositionX * deltaSpeedX + deltaPositionY * deltaSpeedY <= 0) {
				return true;
			}
		}
		return false;
	}

	void Player::solveCircleCollision(Player& other) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float unitNormalVectorX, unitNormalVectorY, unitTangentVectorX, unitTangentVectorY;
		nf::findCoordinatesOfUnitNormalVector(mPositionX, mPositionY, other.getPositionX(), other.getPositionY(), unitNormalVectorX, unitNormalVectorY);
		unitTangentVectorX = unitNormalVectorY;
		unitTangentVectorY = -unitNormalVectorX;

		float thisNormalSpeed = unitNormalVectorX * mSpeedX + unitNormalVectorY * mSpeedY;
		float thisTangentSpeed = unitTangentVectorX * mSpeedX + unitTangentVectorY * mSpeedY;
		float otherNormalSpeed = unitNormalVectorX * other.getSpeedX() + unitNormalVectorY * other.getSpeedY();
		float otherTangentSpeed = unitTangentVectorX * other.getSpeedX() + unitTangentVectorY * other.getSpeedY();

		float thisMass = power(mRadius, 3) * mDensity, otherMass = power(other.getRadius(), 3) * other.getDensity();
		float newThisNormalSpeed, newThisTangentSpeed, newOtherNormalSpeed, newOtherTangentSpeed;
		nf::solveOneDimensionalCollision(thisMass, otherMass, thisNormalSpeed, otherNormalSpeed, newThisNormalSpeed, newOtherNormalSpeed);
		newThisTangentSpeed = thisTangentSpeed;
		newOtherTangentSpeed = otherTangentSpeed;

		float newThisNormalSpeedX = newThisNormalSpeed * unitNormalVectorX, newThisNormalSpeedY = newThisNormalSpeed * unitNormalVectorY;
		float newThisTangentSpeedX = newThisTangentSpeed * unitTangentVectorX, newThisTangentSpeedY = newThisTangentSpeed * unitTangentVectorY;
		float newOtherNormalSpeedX = newOtherNormalSpeed * unitNormalVectorX, newOtherNormalSpeedY = newOtherNormalSpeed * unitNormalVectorY;
		float newOtherTangentSpeedX = newOtherTangentSpeed * unitTangentVectorX, newOtherTangentSpeedY = newOtherTangentSpeed * unitTangentVectorY;

		mSpeedX = newThisNormalSpeedX + newThisTangentSpeedX;
		mSpeedY = newThisNormalSpeedY + newThisTangentSpeedY;
		other.setSpeedX(newOtherNormalSpeedX + newOtherTangentSpeedX);
		other.setSpeedY(newOtherNormalSpeedY + newOtherTangentSpeedY);
	}

	bool Player::circleCollisionDetector(sf::CircleShape circle) {
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
	void Player::solveCircleCollision(sf::CircleShape& circle) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float unitNormalVectorX, unitNormalVectorY, unitTangentVectorX, unitTangentVectorY;
		nf::findCoordinatesOfUnitNormalVector(mPositionX, mPositionY, circle.getPosition().x, circle.getPosition().y, unitNormalVectorX, unitNormalVectorY);
		unitTangentVectorX = unitNormalVectorY;
		unitTangentVectorY = -unitNormalVectorX;

		float playerNormalSpeed = unitNormalVectorX * mSpeedX + unitNormalVectorY * mSpeedY;
		float playerTangentSpeed = unitTangentVectorX * mSpeedX + unitTangentVectorY * mSpeedY;
		float circleNormalSpeed = 0;
		float circleTangentSpeed = 0;

		float playerMass = power(mRadius, 3) * mDensity, circleMass = power(circle.getRadius(), 3) * 1000.f;
		float newPlayerNormalSpeed, newPlayerTangentSpeed, newCircleNormalSpeed, newCircleTangentSpeed;
		nf::solveOneDimensionalCollision(playerMass, circleMass, playerNormalSpeed, circleNormalSpeed, newPlayerNormalSpeed, newCircleNormalSpeed);
		newPlayerTangentSpeed = playerTangentSpeed;

		float newPlayerNormalSpeedX = newPlayerNormalSpeed * unitNormalVectorX, newPlayerNormalSpeedY = newPlayerNormalSpeed * unitNormalVectorY;
		float newPlayerTangentSpeedX = newPlayerTangentSpeed * unitTangentVectorX, newPlayerTangentSpeedY = newPlayerTangentSpeed * unitTangentVectorY;

		mSpeedX = newPlayerNormalSpeedX + newPlayerTangentSpeedX;
		mSpeedY = newPlayerNormalSpeedY + newPlayerTangentSpeedY;
	}
}