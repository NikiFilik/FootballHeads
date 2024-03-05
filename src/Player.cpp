#include "Player.hpp"

#include <cmath>
#include "functions.hpp"

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
	sf::Keyboard::Key Player::getLeftKey(){
		return mLeftKey;
	}
	sf::Keyboard::Key Player::getRightKey() {
		return mRightKey;
	}
	sf::Keyboard::Key Player::getJumpKey() {
		return mJumpKey;
	}

	void Player::moveLeft(sf::Time deltaTime) {
		mSpeedX = std::max(-mMaxSpeed, mSpeedX - (mBoost * deltaTime.asSeconds()));
	}
	void Player::moveRight(sf::Time deltaTime) {
		mSpeedX = std::min(mMaxSpeed, mSpeedX + (mBoost * deltaTime.asSeconds()));
	}
	void Player::doJump() {
		mSpeedY = mJumpForce;
	}

	void Player::update(sf::Time deltaTime, int fieldWidth, int fieldHeight) {
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

		mSpeedX += mAccelerationX;
		mSpeedY += mAccelerationY;

		if (mSpeedX > 0) {
			mSpeedX = std::max(0.f, mSpeedX - mSlowdown);
		}
		if (mSpeedX < 0) {
			mSpeedX = std::min(0.f, mSpeedX + mSlowdown);
		}
	}

	bool Player::leftWallCollisionDetector() {
		if (mPositionX <= mRadius && mSpeedX <= 0) {
			return true;
		}
		return false;
	}
	bool Player::rightWallCollisionDetector(int fieldWidth) {
		if (mPositionX >= fieldWidth - mRadius && mSpeedX >= 0) {
			return true;
		}
		return false;
	}
	bool Player::upWallCollisionDetector() {
		if (mPositionY <= mRadius && mSpeedY <= 0) {
			return true;
		}
		return false;
	}
	bool Player::downWallCollisionDetector(int fieldHeight) {
		if (mPositionY >= fieldHeight - mRadius && mSpeedY >= 0) {
			return true;
		}
		return false;
	}

	void Player::solveLeftWallCollision() {
		mSpeedX *= -mBounceCoefficient;
		mSpeedY *= mBounceCoefficient;
	}
	void Player::solveRightWallCollision() {
		mSpeedX *= -mBounceCoefficient;
		mSpeedY *= mBounceCoefficient;
	}
	void Player::solveUpWallCollision() {
		mSpeedX *= mBounceCoefficient;
		mSpeedY *= -mBounceCoefficient;
	}
	void Player::solveDownWallCollision() {
		mSpeedX *= mBounceCoefficient;
		mSpeedY *= -mBounceCoefficient;
	}

	bool Player::circleCollisionDetector(Player other) {
		if (nf::distanceBetween2Points(mPositionX, mPositionY, other.getPositionX(), other.getPositionY()) <= power(mRadius + other.getRadius(), 2)) {
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
}

/*#include "Player.hpp"
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

	sf::Sprite Player::getSprite() {
		return mSprite;
	}

	void Player::move(sf::Time deltaTime, int width, int height) {
		mPositionX += mSpeedX * deltaTime.asSeconds();
		mPositionY += mSpeedY * deltaTime.asSeconds();

		if (mPositionX <= mRadius) {
			mPositionX = mRadius;
		}
		if (mPositionX >= width - mRadius) {
			mPositionX = width - mRadius;
		}
		if (mPositionY <= mRadius) {
			mPositionY = mRadius;
		}
		if (mPositionY >= height - mRadius) {
			mPositionY = height - mRadius;
		}

		mSprite.setPosition(mPositionX, mPositionY);

		mSpeedY += mAccelerationY * deltaTime.asSeconds();

		if (mSpeedX > 0) {
			mSpeedX = std::max(0.f, mSpeedX - mAccelerationBreak * deltaTime.asSeconds());
		}
		if (mSpeedX < 0) {
			mSpeedX = std::min(0.f, mSpeedX + mAccelerationBreak * deltaTime.asSeconds());
		}

		if (mSpeedX > mMaxSpeedX) {
			mSpeedX = mMaxSpeedX;
		}
		if (mSpeedX < -mMaxSpeedX) {
			mSpeedX = -mMaxSpeedX;
		}
	}

	void Player::moveLeft(sf::Time deltaTime) {
		mSpeedX -= mAccelerationBoost * deltaTime.asSeconds();
	}
	void Player::moveRight(sf::Time deltaTime) {
		mSpeedX += mAccelerationBoost * deltaTime.asSeconds();
	}
	void Player::doJump() {

	}

	void Player::handleWallsCollision(int width, int height) {
		if (mPositionX <= mRadius && mSpeedX < 0) {
			mSpeedX = -mSpeedX * mJumpingCoefficient;
		}
		if (mPositionX >= width - mRadius && mSpeedX > 0) {
			mSpeedX = -mSpeedX * mJumpingCoefficient;
		}
		if (mPositionY <= mRadius && mSpeedY < 0) {
			mSpeedY = -mSpeedY * mJumpingCoefficient;
		}
		if (mPositionY >= height - mRadius && mSpeedY > 0) {
			mSpeedY = -mSpeedY * mJumpingCoefficient;
		}
	}

	bool Player::straightCollisionDetector(sf::Vertex vertex1, sf::Vertex vertex2) {
		float A = vertex2.position.y - vertex1.position.y, B = vertex1.position.x - vertex2.position.x;
		float C = vertex2.position.x * vertex1.position.y - vertex1.position.x * vertex2.position.y;
		float distance = abs(A * mPositionX + B * mPositionY + C) / sqrt(A * A + B * B);
		if (distance <= mRadius) {
			return true;
		}
		return false;
	}

	void Player::handleStraightCollision(sf::Vertex vertex1, sf::Vertex vertex2) {
		float normalVectorX = vertex2.position.x - vertex1.position.x;
		float normalVectorY = vertex2.position.y - vertex1.position.y;

		float unitNormalVectorX = normalVectorX / sqrt(nf::power(normalVectorX, 2) + nf::power(normalVectorY, 2));
		float unitNormalVectorY = normalVectorY / sqrt(nf::power(normalVectorX, 2) + nf::power(normalVectorY, 2));
		float unitTangentVectorX = unitNormalVectorY;
		float unitTangentVectorY = -unitNormalVectorX;

		float normalSpeed = unitNormalVectorX * mSpeedX + unitNormalVectorY * mSpeedY;
		float tangentSpeed = -(unitTangentVectorX * mSpeedX + unitTangentVectorY * mSpeedY);

		float normalSpeedX = normalSpeed * unitNormalVectorX;
		float normalSpeedY = normalSpeed * unitNormalVectorY;
		float tangentSpeedX = tangentSpeed * unitTangentVectorX;
		float tangentSpeedY = tangentSpeed * unitTangentVectorY;

		mSpeedX = (normalSpeedX + tangentSpeedX) * mSlipCoefficient;
		mSpeedY = (normalSpeedY + tangentSpeedY) * mJumpingCoefficient;
	}

	bool Player::circleCollisionDetector(Player player) {
		if (power(mPositionX - player.getPositionX(), 2) + power(mPositionY - player.getPositionY(), 2) <= power(mRadius + player.getRadius(), 2)) {
			float deltaPositionX = mPositionX - player.getPositionX();
			float deltaPositionY = mPositionY - player.getPositionY();
			float deltaVelocityX = mSpeedX - player.getSpeedX();
			float deltaVelocityY = mSpeedY - player.getSpeedY();
			if (deltaPositionX * deltaVelocityX + deltaPositionY * deltaVelocityY <= 0) {
				return true;
			}
		}
		return false;
	}

	void Player::handleCircleCollision(Player& player) {
		float x1 = mPositionX, y1 = mPositionY, x2 = player.getPositionX(), y2 = player.getPositionY();
		float nx = x2 - x1, ny = y2 - y1;
		float unx = nx / std::sqrt(nx * nx + ny * ny), uny = ny / std::sqrt(nx * nx + ny * ny);
		float utx = uny, uty = -unx;

		float v1x = mSpeedX, v1y = mSpeedY, v2x = player.getSpeedX(), v2y = player.getSpeedY();

		float v1n = unx * v1x + uny * v1y, v1t = utx * v1x + uty * v1y;
		float v2n = unx * v2x + uny * v2y, v2t = utx * v2x + uty * v2y;

		float V1t = v1t, V2t = v2t;

		float m1 = power(mRadius, 3), m2 = power(player.getRadius(), 3);
		float V1n = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
		float V2n = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

		float V1nx = V1n * unx, V1ny = V1n * uny;
		float V1tx = V1t * utx, V1ty = V1t * uty;
		float V2nx = V2n * unx, V2ny = V2n * uny;
		float V2tx = V2t * utx, V2ty = V2t * uty;

		float V1x = V1nx + V1tx, V1y = V1ny + V1ty;
		float V2x = V2nx + V2tx, V2y = V2ny + V2ty;

		mSpeedX = V1x;
		mSpeedY = V1y;
		player.setSpeedX(V2x);
		player.setSpeedY(V2y);
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

	sf::Keyboard::Key Player::getLeftKey() {
		return mLeftKey;
	}
	sf::Keyboard::Key Player::getRightKey() {
		return mRightKey;
	}
	sf::Keyboard::Key Player::getJumpKey() {
		return mJumpKey;
	}

	void Player::setSpeedX(float speed) {
		mSpeedX = speed;
	}
	void Player::setSpeedY(float speed) {
		mSpeedY = speed;
	}
}*/