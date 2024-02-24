#include "Ball.hpp"
#include "functions.hpp"

#include <cmath>

namespace nf {
	void Ball::setup(float positionX, float positionY, float radius) {
		this->positionX = positionX;
		this->positionY = positionY;
		velocityX = nf::randIntFromRange(-400, 401);
		velocityY = nf::randIntFromRange(-400, 401);
		accelerationX = 0;
		accelerationY = 0.3;
		this->radius = radius;
		shape.setRadius(radius);
		shape.setOrigin(radius, radius);
		shape.setPosition(positionX, positionY);
		shape.setFillColor(sf::Color::White);
	}

	sf::CircleShape Ball::getShape() {
		return shape;
	}

	void Ball::move(float deltaTime, int width, int height) {
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

		velocityX = velocityX * 0.99999 + accelerationX;
		velocityY = velocityY * 0.99999 + accelerationY;
	}

	bool Ball::leftWallCollisionDetector() {
		if (positionX <= radius && velocityX <= 0) {
			return true;
		}
		return false;
	}
	bool Ball::rightWallCollisionDetector(int width) {
		if (positionX >= width - radius && velocityX >= 0) {
			return true;
		}
		return false;
	}
	bool Ball::upWallCollisionDetector() {
		if (positionY <= radius && velocityY <= 0) {
			return true;
		}
		return false;
	}
	bool Ball::downWallCollisionDetector(int height) {
		if (positionY >= height - radius && velocityY >= 0) {
			return true;
		}
		return false;
	}

	void Ball::calculateLeftWallCollision() {
		velocityX = -velocityX * 0.7;
	}
	void Ball::calculateRightWallCollision() {
		velocityX = -velocityX * 0.7;
	}
	void Ball::calculateUpWallCollision() {
		velocityY = -velocityY * 0.7;
	}
	void Ball::calculateDownWallCollision() {
		velocityY = -velocityY * 0.7;
	}

	bool Ball::playerCollisionDetector(Player player) {
		if (power(positionX - player.getPositionX(), 2) + power(positionY - player.getPositionY(), 2) <= power(radius + player.getRadius(), 2)) {
			float deltaPositionX = positionX - player.getPositionX();
			float deltaPositionY = positionY - player.getPositionY();
			float deltaVelocityX = velocityX - player.getVelocityX();
			float deltaVelocityY = velocityY - player.getVelocityY();
			if (deltaPositionX * deltaVelocityX + deltaPositionY * deltaVelocityY <= 0) {
				return true;
			}
		}
		return false;
	}

	void Ball::calculatePlayerCollision(Player& player) {
		//https://www.vobarian.com/collisions/2dcollisions2.pdf

		float x1 = positionX, y1 = positionY, x2 = player.getPositionX(), y2 = player.getPositionY();
		float nx = x2 - x1, ny = y2 - y1;
		float unx = nx / std::sqrt(nx * nx + ny * ny), uny = ny / std::sqrt(nx * nx + ny * ny);
		float utx = uny, uty = -unx;

		float v1x = velocityX, v1y = velocityY, v2x = player.getVelocityX(), v2y = player.getVelocityY();

		float v1n = unx * v1x + uny * v1y, v1t = utx * v1x + uty * v1y;
		float v2n = unx * v2x + uny * v2y, v2t = utx * v2x + uty * v2y;

		float V1t = v1t, V2t = v2t;

		float m1 = power(radius, 3), m2 = power(player.getRadius(), 3);
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
		player.setVelocityX(V2x);
		player.setVelocityY(V2y);
	}
}