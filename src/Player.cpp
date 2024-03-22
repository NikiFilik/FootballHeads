#include "Player.hpp"
#include "functions.hpp"

#include <cmath>

namespace nf {
	void Player::setup(float positionX, float positionY, std::string fileName, std::string legFileName, sf::Keyboard::Key leftKey, sf::Keyboard::Key rightKey, sf::Keyboard::Key jumpKey, sf::Keyboard::Key kickKey, int side) {
		mPositionX = positionX;
		mPositionY = positionY;
		mTexture.loadFromFile(fileName);
		mSprite.setTexture(mTexture);
		mSprite.setOrigin(mRadius, mRadius);
		mSprite.setPosition(mPositionX, mPositionY);
		mLeftKey = leftKey;
		mRightKey = rightKey;
		mJumpKey = jumpKey;
		mKickKey = kickKey;
		mSide = side;

		mLegTexture.loadFromFile(legFileName);
		mLegSprite.setTexture(mLegTexture);
		mLegSprite.setOrigin(mLegRadius, -mRadius * 0.9f);
		mLegSprite.setPosition(mPositionX, mPositionY);
	}
	void Player::restart(float positionX, float positionY) {
		mPositionX = positionX;
		mPositionY = positionY;
		mSpeedX = 0.f;
		mSpeedY = 0.f;
		mLegSprite.setRotation(0);
		mLegRotationSpeed = 0.f;
		legUpdate();
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
	sf::Keyboard::Key Player::getKickKey() {
		return mKickKey;
	}
	sf::Sprite Player::getLegSprite() {
		return mLegSprite;
	}
	float Player::getLegPositionX() {
		return mLegPositionX;
	}
	float Player::getLegPositionY() {
		return mLegPositionY;
	}
	float Player::getLegSpeedX() {
		return mLegSpeedX;
	}
	float Player::getLegSpeedY() {
		return mLegSpeedY;
	}
	float Player::getLegRadius() {
		return mLegRadius;
	}

	void Player::moveLeft(sf::Time deltaTime) {
		mSpeedX = std::max(-mMaxSpeed, mSpeedX - mBoost * deltaTime.asSeconds());
		legUpdate();
	}
	void Player::moveRight(sf::Time deltaTime) {
		mSpeedX = std::min(mMaxSpeed, mSpeedX + mBoost * deltaTime.asSeconds());
		legUpdate();
	}
	void Player::doKick(sf::Time deltaTime) {
		if (mSide == 1) {
			mLegRotationSpeed = std::min(mLegMaxRotationSpeed, mLegRotationSpeed + mBoost * deltaTime.asSeconds());
			legUpdate();
		}
		else {
			mLegRotationSpeed = std::max(mLegMaxRotationSpeed * mSide, mLegRotationSpeed + mBoost * deltaTime.asSeconds() * mSide);
			legUpdate();
		}
	}
	void Player::doJump() {
		mSpeedY = -mJumpForce;
		legUpdate();
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
		mLegSprite.setPosition(mPositionX, mPositionY);

		mSpeedX += mAccelerationX * deltaTime.asSeconds();
		mSpeedY += mAccelerationY * deltaTime.asSeconds();

		mLegSprite.rotate(mLegRotationSpeed * deltaTime.asSeconds());
		if (mSide == 1) {
			if (mLegSprite.getRotation() >= 90.f && mLegSprite.getRotation() <= 120.f) {
				mLegSprite.setRotation(90.f);
				mLegRotationSpeed *= 0.1f;
			}
			if (mLegSprite.getRotation() <= 315.f && mLegSprite.getRotation() >= 285.f) {
				mLegSprite.setRotation(315.f);
				mLegRotationSpeed *= 0.1f;
			}
		}
		else {
			if (mLegSprite.getRotation() >= 45.f && mLegSprite.getRotation() <= 75.f) {
				mLegSprite.setRotation(45.f);
				mLegRotationSpeed *= 0.1f;
			}
			if (mLegSprite.getRotation() <= 270.f && mLegSprite.getRotation() >= 240.f) {
				mLegSprite.setRotation(270.f);
				mLegRotationSpeed *= 0.1f;
			}
		}

		if (mSpeedX > 0) {
			mSpeedX = std::max(0.f, mSpeedX - mSlowdown * deltaTime.asSeconds());
		}
		if (mSpeedX < 0) {
			mSpeedX = std::min(0.f, mSpeedX + mSlowdown * deltaTime.asSeconds());
		}

		if (mSide == 1) {
			mLegRotationSpeed = std::min(mLegMaxRotationSpeed, mLegRotationSpeed + -mLegSlowdown * deltaTime.asSeconds());
			legUpdate();
		}
		else {
			mLegRotationSpeed = std::max(mLegMaxRotationSpeed * mSide, mLegRotationSpeed + -mLegSlowdown * deltaTime.asSeconds() * mSide);
			legUpdate();
		}
		if (mLegRotationSpeed > mLegMaxRotationSpeed) {
			mLegRotationSpeed = mLegMaxRotationSpeed;
		}
		if (mLegRotationSpeed < -mLegMaxRotationSpeed) {
			mLegRotationSpeed = -mLegMaxRotationSpeed;
		}
		legUpdate();
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
		legUpdate();
	}
	void Player::solveRightStraightCollision() {
		mSpeedX *= -mBounceCoefficient;
		legUpdate();
	}
	void Player::solveUpStraightCollision() {
		mSpeedY *= -mBounceCoefficient;
		legUpdate();
	}
	void Player::solveDownStraightCollision() {
		mSpeedY *= -mBounceCoefficient;
		legUpdate();
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
		legUpdate();
		other.legUpdate();
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
		legUpdate();
	}

	void Player::legUpdate() {
		if (mSide == 1) {
			if (mLegSprite.getRotation() >= 0.f && mLegSprite.getRotation() <= 270.f) {
				mLegPositionX = mPositionX + std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegPositionY = mPositionY - std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegSpeedX = mSpeedX + std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
				mLegSpeedY = mSpeedY - std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
			}
			else {
				mLegPositionX = mPositionX + std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegPositionY = mPositionY - std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegSpeedX = mSpeedX + std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
				mLegSpeedY = mSpeedY - std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
			}
		}
		else {
			if (mLegSprite.getRotation() >= 0.f && mLegSprite.getRotation() <= 270.f) {
				mLegPositionX = mPositionX + std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegPositionY = mPositionY - std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegSpeedX = mSpeedX - std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
				mLegSpeedY = mSpeedY + std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
			}
			else {
				mLegPositionX = mPositionX + std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegPositionY = mPositionY - std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * (mLegRadius + mRadius * 0.9f);
				mLegSpeedX = mSpeedX - std::cos((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
				mLegSpeedY = mSpeedY + std::sin((270.f - mLegSprite.getRotation()) * 3.14f / 180.f) * mLegRotationSpeed * (mLegRadius + mRadius * 0.9f) * 2.f * 3.14f / 360.f;
			}
		}
	}
}