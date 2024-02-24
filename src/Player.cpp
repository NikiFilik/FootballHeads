#include "Player.hpp"
#include "functions.hpp"

#include <cmath>

namespace nf {
	void Player::setup(float positionX, float positionY, float radius, int redColor, int greenColor, int blueColor) {
		this->positionX = positionX;
		this->positionY = positionY;
		velocityX = 0;
		velocityY = 0;
		accelerationX = 0;
		accelerationY = 0.3;
		this->radius = radius;
		shape.setRadius(radius);
		shape.setOrigin(radius, radius);
		shape.setPosition(positionX, positionY);
		shape.setFillColor(sf::Color(redColor, greenColor, blueColor, 255));
	}

	void Player::setVelocityX(float velocityX) {
		this->velocityX = velocityX;
	}
	void Player::setVelocityY(float velocityY) {
		this->velocityY = velocityY;
	}

	float Player::getPositionX() {
		return positionX;
	}
	float Player::getPositionY() {
		return positionY;
	}
	float Player::getVelocityX() {
		return velocityX;
	}
	float Player::getVelocityY() {
		return velocityY;
	}
	float Player::getRadius() {
		return radius;
	}
	sf::CircleShape Player::getShape() {
		return shape;
	}

	void Player::leftButtonPressed() {
		velocityX = std::max(-600.f, velocityX - 1);
	}
	void Player::rightButtonPressed() {
		velocityX = std::min(600.f, velocityX + 1);
	}
	void Player::jumpButtonPressed() {
		velocityY -= 1000;
	}

	void Player::move(float deltaTime, int width, int height) {
		positionX += velocityX * deltaTime + (accelerationX * deltaTime * deltaTime) / 2;
		positionY += velocityY * deltaTime + (accelerationY * deltaTime * deltaTime) / 2;

		if (positionX - radius < 0) {
			positionX = radius;
		}
		if (positionX + radius > width) {
			positionX = width - radius;
		}
		if (positionY - radius < 0) {
			positionY = radius;
		}
		if (positionY + radius > height) {
			positionY = height - radius;
		}

		shape.setPosition(positionX, positionY);

		velocityX = velocityX + accelerationX;
		velocityY = velocityY + accelerationY;

		if (velocityX > 0) {
			velocityX = std::max(0.f, velocityX - 0.5f);
		}
		if (velocityX < 0) {
			velocityX = std::min(0.f, velocityX + 0.5f);
		}
	}

	bool Player::leftWallCollisionDetector() {
		if (positionX <= radius && velocityX <= 0) {
			return true;
		}
		return false;
	}
	bool Player::rightWallCollisionDetector(int width) {
		if (positionX >= width - radius && velocityX >= 0) {
			return true;
		}
		return false;
	}
	bool Player::upWallCollisionDetector() {
		if (positionY <= radius && velocityY <= 0) {
			return true;
		}
		return false;
	}
	bool Player::downWallCollisionDetector(int height) {
		if (positionY >= height - radius && velocityY >= 0) {
			return true;
		}
		return false;
	}

	void Player::calculateLeftWallCollision() {
		velocityX = 0;
	}
	void Player::calculateRightWallCollision() {
		velocityX = 0;
	}
	void Player::calculateUpWallCollision() {
		velocityY = 0;
	}
	void Player::calculateDownWallCollision() {
		velocityY = 0;
	}

	bool Player::playerCollisionDetector(Player other) {
		if (power(positionX - other.getPositionX(), 2) + power(positionY - other.getPositionY(), 2) <= power(radius + other.getRadius(), 2)) {
			float deltaPositionX = positionX - other.getPositionX();
			float deltaPositionY = positionY - other.getPositionY();
			float deltaVelocityX = velocityX - other.getVelocityX();
			float deltaVelocityY = velocityY - other.getVelocityY();
			if (deltaPositionX * deltaVelocityX + deltaPositionY * deltaVelocityY <= 0) {
				return true;
			}
		}
		return false;
	}

	void Player::calculatePlayerCollision(Player& other) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float x1 = positionX, y1 = positionY, x2 = other.getPositionX(), y2 = other.getPositionY();
		float nx = x2 - x1, ny = y2 - y1;
		float unx = nx / std::sqrt(nx * nx + ny * ny), uny = ny / std::sqrt(nx * nx + ny * ny);
		float utx = uny, uty = -unx;

		float v1x = velocityX, v1y = velocityY, v2x = other.getVelocityX(), v2y = other.getVelocityY();

		float v1n = unx * v1x + uny * v1y, v1t = utx * v1x + uty * v1y;
		float v2n = unx * v2x + uny * v2y, v2t = utx * v2x + uty * v2y;

		float V1t = v1t, V2t = v2t;

		float m1 = power(radius, 3), m2 = power(other.getRadius(), 3);
		float V1n = (v1n * (m1 - m2) + 2 * m2 * v2n) / (m1 + m2);
		float V2n = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);

		float V1nx = V1n * unx, V1ny = V1n * uny;
		float V1tx = V1t * utx, V1ty = V1t * uty;
		float V2nx = V2n * unx, V2ny = V2n * uny;
		float V2tx = V2t * utx, V2ty = V2t * uty;

		float V1x = V1nx + V1tx, V1y = V1ny + V1ty;
		float V2x = V2nx + V2tx, V2y = V2ny + V2ty;

		velocityX = V1x;
		velocityY = V1y;
		other.velocityX = V2x;
		other.velocityY = V2y;
	}
}