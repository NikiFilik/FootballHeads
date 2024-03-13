#include "Game.hpp"

namespace nf {
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	Game::Game() : mWindow(sf::VideoMode(mWidth, mHeight), mTitle, sf::Style::Fullscreen) {
		mWindow.setVerticalSyncEnabled(true);

		mBall.setup(960.f, 540.f, "ball.png");

		mPlayer1.setup(1440.f, 840.f, "player1.png", sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up);
		mPlayer2.setup(480.f, 840.f, "player2.png", sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W);

		mBackgroundTexture.loadFromFile("background.png");
		mBackground.setTexture(mBackgroundTexture);


		mGatesTexture.loadFromFile("gates.png");
		mGates.setTexture(mGatesTexture);
		mCrossbar1.setRadius(mCrossbarRadius);
		mCrossbar2.setRadius(mCrossbarRadius);
		mCrossbar1.setOrigin(mCrossbarRadius, mCrossbarRadius);
		mCrossbar2.setOrigin(mCrossbarRadius, mCrossbarRadius);
		mCrossbar1.setPosition(200.f, 660.f);
		mCrossbar2.setPosition(1720.f, 660.f);
		mCrossbar1.setFillColor(sf::Color::White);
		mCrossbar2.setFillColor(sf::Color::White);
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
		if (mBall.leftStraightCollisionDetector(0.f)) {
			mBall.solveLeftStraightCollision();
		}
		if (mBall.rightStraightCollisionDetector(mFieldWidth)) {
			mBall.solveRightStraightCollision();
		}
		if (mBall.upStraightCollisionDetector(0.f)) {
			mBall.solveUpStraightCollision();
		}
		if (mBall.downStraightCollisionDetector(mFieldHeight)) {
			mBall.solveDownStraightCollision();
		}

		if (mPlayer1.leftStraightCollisionDetector(0.f)) {
			mPlayer1.solveLeftStraightCollision();
		}
		if (mPlayer1.rightStraightCollisionDetector(mFieldWidth)) {
			mPlayer1.solveRightStraightCollision();
		}
		if (mPlayer1.upStraightCollisionDetector(0.f)) {
			mPlayer1.solveUpStraightCollision();
		}
		if (mPlayer1.downStraightCollisionDetector(mFieldHeight)) {
			mPlayer1.solveDownStraightCollision();
		}

		if (mPlayer2.leftStraightCollisionDetector(0.f)) {
			mPlayer2.solveLeftStraightCollision();
		}
		if (mPlayer2.rightStraightCollisionDetector(mFieldWidth)) {
			mPlayer2.solveRightStraightCollision();
		}
		if (mPlayer2.upStraightCollisionDetector(0.f)) {
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



		if (mBall.circleCollisionDetector(mCrossbar1)) {
			mBall.solveCircleCollision(mCrossbar1);
		}
		if (mBall.circleCollisionDetector(mCrossbar2)) {
			mBall.solveCircleCollision(mCrossbar2);
		}



		if (mPlayer1.circleCollisionDetector(mPlayer2)) {
			mPlayer1.solveCircleCollision(mPlayer2);
		}



		if (mPlayer1.circleCollisionDetector(mCrossbar1)) {
			mPlayer1.solveCircleCollision(mCrossbar1);
		}
		if (mPlayer1.circleCollisionDetector(mCrossbar2)) {
			mPlayer1.solveCircleCollision(mCrossbar2);
		}
		if (mPlayer2.circleCollisionDetector(mCrossbar1)) {
			mPlayer2.solveCircleCollision(mCrossbar1);
		}
		if (mPlayer2.circleCollisionDetector(mCrossbar2)) {
			mPlayer2.solveCircleCollision(mCrossbar2);
		}



		if (mBall.downStraightCollisionDetector(mCrossbar1PositionY - mCrossbarRadius)) {
			if (mBall.getPositionX() <= mCrossbar1PositionX) {
				mBall.solveUpStraightCollision();
			}
		}
		if (mBall.downStraightCollisionDetector(mCrossbar2PositionY - mCrossbarRadius)) {
			if (mBall.getPositionX() >= mCrossbar2PositionX) {
				mBall.solveUpStraightCollision();
			}
		}
		if (mBall.upStraightCollisionDetector(mCrossbar1PositionY + mCrossbarRadius)) {
			if (mBall.getPositionX() <= mCrossbar1PositionX) {
				mBall.solveDownStraightCollision();
			}
		}
		if (mBall.upStraightCollisionDetector(mCrossbar2PositionY + mCrossbarRadius)) {
			if (mBall.getPositionX() >= mCrossbar2PositionX) {
				mBall.solveDownStraightCollision();
			}
		}

		if (mPlayer1.downStraightCollisionDetector(mCrossbar1PositionY - mCrossbarRadius)) {
			if (mPlayer1.getPositionX() <= mCrossbar1PositionX) {
				mPlayer1.solveUpStraightCollision();
			}
		}
		if (mPlayer1.downStraightCollisionDetector(mCrossbar2PositionY - mCrossbarRadius)) {
			if (mPlayer1.getPositionX() >= mCrossbar2PositionX) {
				mPlayer1.solveUpStraightCollision();
			}
		}
		if (mPlayer1.upStraightCollisionDetector(mCrossbar1PositionY + mCrossbarRadius)) {
			if (mPlayer1.getPositionX() <= mCrossbar1PositionX) {
				mPlayer1.solveDownStraightCollision();
			}
		}
		if (mPlayer1.upStraightCollisionDetector(mCrossbar2PositionY + mCrossbarRadius)) {
			if (mPlayer1.getPositionX() >= mCrossbar2PositionX) {
				mPlayer1.solveDownStraightCollision();
			}
		}

		if (mPlayer2.downStraightCollisionDetector(mCrossbar1PositionY - mCrossbarRadius)) {
			if (mPlayer2.getPositionX() <= mCrossbar1PositionX) {
				mPlayer2.solveUpStraightCollision();
			}
		}
		if (mPlayer2.downStraightCollisionDetector(mCrossbar2PositionY - mCrossbarRadius)) {
			if (mPlayer2.getPositionX() >= mCrossbar2PositionX) {
				mPlayer2.solveUpStraightCollision();
			}
		}
		if (mPlayer2.upStraightCollisionDetector(mCrossbar1PositionY + mCrossbarRadius)) {
			if (mPlayer2.getPositionX() <= mCrossbar1PositionX) {
				mPlayer2.solveDownStraightCollision();
			}
		}
		if (mPlayer2.upStraightCollisionDetector(mCrossbar2PositionY + mCrossbarRadius)) {
			if (mPlayer2.getPositionX() >= mCrossbar2PositionX) {
				mPlayer2.solveDownStraightCollision();
			}
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

		mWindow.draw(mGates);

		mWindow.display();
	}
}