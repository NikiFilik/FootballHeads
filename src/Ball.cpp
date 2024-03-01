#include "Ball.hpp"
#include "functions.hpp"

namespace nf {
	void Ball::setup(float positionX, float positionY) {
		mPositionX = positionX;
		mPositionY = positionY;
		mSpeedX = nf::randIntFromRange(-400, 401);
		mSpeedY = nf::randIntFromRange(-400, 401);

		mTexture.loadFromFile("../media/textures/ball.png");
		mSprite.setTexture(mTexture);
		mSprite.setOrigin(mRadius, mRadius);
		mSprite.setPosition(mPositionX, mPositionY);
	}

	sf::Sprite Ball::getSprite() {
		return mSprite;
	}

	void Ball::move(sf::Time deltaTime, int width, int height) {
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

		mSprite.rotate(mSpeedX * deltaTime.asSeconds() / (mRadius * 6.28 / 360));
		mSprite.setPosition(mPositionX, mPositionY);

		mSpeedX += mAccelerationX * deltaTime.asSeconds();
		mSpeedY += mAccelerationY * deltaTime.asSeconds();
	}

	void Ball::handleWallsCollision(int width, int height) {
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

	/*bool Ball::straightCollisionDetector(sf::Vertex vertex1, sf::Vertex vertex2) {
		float A = vertex2.position.y - vertex1.position.y, B = vertex1.position.x - vertex2.position.x;
		float C = vertex2.position.x * vertex1.position.y - vertex1.position.x * vertex2.position.y;
		float distance = abs(A * mPositionX + B * mPositionY + C) / sqrt(A * A + B * B);
		if (distance <= mRadius) {
			if ((-A * mPositionX - B * mPositionY - C) / (A * mSpeedX + B * mSpeedY) >= 0) {
				return true;
			}
		}
		return false;
	}

	void Ball::handleStraightCollision(sf::Vertex vertex1, sf::Vertex vertex2) {
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

		mSpeedX = (normalSpeedX + tangentSpeedX) * mJumpingCoefficient;
		mSpeedY = (normalSpeedY + tangentSpeedY) * mJumpingCoefficient;
	}*/

	bool Ball::circleCollisionDetector(Player player) {
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

	void Ball::handleCircleCollision(Player& player) {
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
}