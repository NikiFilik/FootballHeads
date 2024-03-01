#include "Game.hpp"

#include <iostream>

namespace nf {
	const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);

	Game::Game() : mWindow(sf::VideoMode(mWidth, mHeight), mTitle, sf::Style::Fullscreen) {
		/*mField.setup(11);
		mField[0].position = sf::Vector2f(60.f, 960.f);
		mField[1].position = sf::Vector2f(60.f, 600.f);
		mField[2].position = sf::Vector2f(120.f, 360.f);
		mField[3].position = sf::Vector2f(360.f, 120.f);
		mField[4].position = sf::Vector2f(600.f, 60.f);
		mField[5].position = sf::Vector2f(1320.f, 60.f);
		mField[6].position = sf::Vector2f(1560.f, 120.f);
		mField[7].position = sf::Vector2f(1800.f, 360.f);
		mField[8].position = sf::Vector2f(1860.f, 600.f);
		mField[9].position = sf::Vector2f(1860.f, 960.f);
		mField[10].position = sf::Vector2f(60.f, 960.f);*/

		mBall.setup(960, 540);

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
				processEvents(TimePerFrame);
				timeSinceLastUpdate += clock.restart();
				while (timeSinceLastUpdate > TimePerFrame)
				{
					timeSinceLastUpdate -= TimePerFrame;
					processEvents(TimePerFrame);
					update(TimePerFrame);
				}
				render();
			}
		}
	}

	void Game::processEvents(sf::Time deltaTime) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			mWindow.close();
		}
		if (sf::Keyboard::isKeyPressed(mPlayer1.getLeftKey())) {
			mPlayer1.moveLeft(deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer1.getRightKey())) {
			mPlayer1.moveRight(deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer1.getJumpKey())) {

		}
		if (sf::Keyboard::isKeyPressed(mPlayer2.getLeftKey())) {
			mPlayer2.moveLeft(deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer2.getRightKey())) {
			mPlayer2.moveRight(deltaTime);
		}
		if (sf::Keyboard::isKeyPressed(mPlayer2.getJumpKey())) {

		}
	}

	void Game::update(sf::Time deltaTime) {
		/*for (int i = 0; i < 10; i++) {
			if (mBall.straightCollisionDetector(mField[i], mField[i + 1])) {
				mBall.handleStraightCollision(mField[i], mField[i + 1]);
			}
			if (mPlayer1.straightCollisionDetector(mField[i], mField[i + 1])) {
				mPlayer1.handleStraightCollision(mField[i], mField[i + 1]);
			}
			if (mPlayer2.straightCollisionDetector(mField[i], mField[i + 1])) {
				mPlayer2.handleStraightCollision(mField[i], mField[i + 1]);
			}
		}*/
		mBall.handleWallsCollision(mFieldWidth, mFieldHeight);
		mPlayer1.handleWallsCollision(mFieldWidth, mFieldHeight);
		mPlayer2.handleWallsCollision(mFieldWidth, mFieldHeight);

		if (mBall.circleCollisionDetector(mPlayer1)) {
			mBall.handleCircleCollision(mPlayer1);
		}
		if (mBall.circleCollisionDetector(mPlayer2)) {
			mBall.handleCircleCollision(mPlayer2);
		}
		if (mPlayer1.circleCollisionDetector(mPlayer2)) {
			mPlayer1.handleCircleCollision(mPlayer2);
		}

		//mPlayer1.update(mField[9], mField[10], mPlayer2);
		//mPlayer2.update(mField[9], mField[10], mPlayer1);

		mBall.move(deltaTime, mFieldWidth, mFieldHeight);
		mPlayer1.move(deltaTime, mFieldWidth, mFieldHeight);
		mPlayer2.move(deltaTime, mFieldWidth, mFieldHeight);
	}

	void Game::render() {
		mWindow.clear();
		
		//mWindow.draw(mField.getShape(), 11, sf::LineStrip);
		mWindow.draw(mBackground);

		mWindow.draw(mBall.getSprite());
		mWindow.draw(mPlayer1.getSprite());
		mWindow.draw(mPlayer2.getSprite());

		mWindow.display();
	}
}