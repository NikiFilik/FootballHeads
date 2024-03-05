#include "Game.hpp"

namespace nf {
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	Game::Game() : mWindow(sf::VideoMode(mWidth, mHeight), mTitle, sf::Style::Fullscreen) {
		mBall.setup(960, 540, "../media/textures/ball.png");

		mPlayer1.setup(1440, 840, "../media/textures/player1.png", sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up);
		mPlayer2.setup(480, 840, "../media/textures/player2.png", sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W);

		mBackgroundTexture.loadFromFile("../media/textures/background.png");
		mBackground.setTexture(mBackgroundTexture);
	}

	void Game::run() {
		while (mWindow.isOpen())
		{
			sf::Clock clock;
			sf::Time timeSinceLastUpdate = sf::Time::Zero;
			while (mWindow.isOpen())
			{
				timeSinceLastUpdate += clock.restart();
				if (timeSinceLastUpdate >= timePerFrame) {
					timeSinceLastUpdate -= timePerFrame;
					processInput();
					update();
					render();
				}
			}
		}
	}

	void Game::processInput() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			mWindow.close();
		}

		if (sf::Keyboard::isKeyPressed(mPlayer1.getLeftKey())) {
			mPlayer1.moveLeft(timePerFrame);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer1.getRightKey())) {
			mPlayer1.moveRight(timePerFrame);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer1.getJumpKey())) {
			if (mPlayer1.downStraightCollisionDetector(mFieldHeight) || (mPlayer1.circleCollisionDetector(mPlayer2) && mPlayer2.downStraightCollisionDetector(mFieldHeight))) {
				mPlayer1.doJump();
			}
		}

		if (sf::Keyboard::isKeyPressed(mPlayer2.getLeftKey())) {
			mPlayer2.moveLeft(timePerFrame);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer2.getRightKey())) {
			mPlayer2.moveRight(timePerFrame);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer2.getJumpKey())) {
			if (mPlayer2.downStraightCollisionDetector(mFieldHeight) || (mPlayer2.circleCollisionDetector(mPlayer1) && mPlayer1.downStraightCollisionDetector(mFieldHeight))) {
				mPlayer2.doJump();
			}
		}
	}

	void Game::update() {
		if (mBall.leftStraightCollisionDetector()) {
			mBall.solveLeftStraightCollision();
		}
		if (mBall.rightStraightCollisionDetector(mFieldWidth)) {
			mBall.solveRightStraightCollision();
		}
		if (mBall.upStraightCollisionDetector()) {
			mBall.solveUpStraightCollision();
		}
		if (mBall.downStraightCollisionDetector(mFieldHeight)) {
			mBall.solveDownStraightCollision();
		}

		if (mPlayer1.leftStraightCollisionDetector()) {
			mPlayer1.solveLeftStraightCollision();
		}
		if (mPlayer1.rightStraightCollisionDetector(mFieldWidth)) {
			mPlayer1.solveRightStraightCollision();
		}
		if (mPlayer1.upStraightCollisionDetector()) {
			mPlayer1.solveUpStraightCollision();
		}
		if (mPlayer1.downStraightCollisionDetector(mFieldHeight)) {
			mPlayer1.solveDownStraightCollision();
		}

		if (mPlayer2.leftStraightCollisionDetector()) {
			mPlayer2.solveLeftStraightCollision();
		}
		if (mPlayer2.rightStraightCollisionDetector(mFieldWidth)) {
			mPlayer2.solveRightStraightCollision();
		}
		if (mPlayer2.upStraightCollisionDetector()) {
			mPlayer2.solveUpStraightCollision();
		}
		if (mPlayer2.downStraightCollisionDetector(mFieldHeight)) {
			mPlayer2.solveDownStraightCollision();
		}

		if (mBall.circleCollisionDetector(mPlayer1)) {
			mBall.solveCircleCollision(mPlayer1);
		}
		if (mBall.circleCollisionDetector(mPlayer2)) {
			mBall.solveCircleCollision(mPlayer2);
		}
		if (mPlayer1.circleCollisionDetector(mPlayer2)) {
			mPlayer1.solveCircleCollision(mPlayer2);
		}

		mBall.update(timePerFrame, mFieldWidth, mFieldHeight);
		mPlayer1.update(timePerFrame, mFieldWidth, mFieldHeight);
		mPlayer2.update(timePerFrame, mFieldWidth, mFieldHeight);
	}

	void Game::render() {
		mWindow.clear();

		mWindow.draw(mBackground);
		mWindow.draw(mBall.getSprite());
		mWindow.draw(mPlayer1.getSprite());
		mWindow.draw(mPlayer2.getSprite());

		mWindow.display();
	}
}