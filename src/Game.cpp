#include <iostream>

#include "Game.hpp"

namespace nf {
	Game::Game(int width, int height, std::string title) {
		this->width = width;
		this->height = height;
		this->title = title;
	}
	void Game::setup() {
		ball.setup(800, 450, 25);
		player1.setup(1200, 600, 50, 255, 127, 63);
		player2.setup(400, 600, 50, 63, 127, 255);
		window.create(sf::VideoMode(width, height), title);
	}
	void Game::lifeCycle() {
		clock.restart();
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear(sf::Color::Black);

			if (ball.leftWallCollisionDetector()) {
				ball.calculateLeftWallCollision();
			}
			if (ball.rightWallCollisionDetector(width)) {
				ball.calculateRightWallCollision();
			}
			if (ball.upWallCollisionDetector()) {
				ball.calculateUpWallCollision();
			}
			if (ball.downWallCollisionDetector(height)) {
				ball.calculateDownWallCollision();
			}

			if (player1.leftWallCollisionDetector()) {
				player1.calculateLeftWallCollision();
			}
			if (player1.rightWallCollisionDetector(width)) {
				player1.calculateRightWallCollision();
			}
			if (player1.upWallCollisionDetector()) {
				player1.calculateUpWallCollision();
			}
			if (player1.downWallCollisionDetector(height)) {
				player1.calculateDownWallCollision();
			}

			if (player2.leftWallCollisionDetector()) {
				player2.calculateLeftWallCollision();
			}
			if (player2.rightWallCollisionDetector(width)) {
				player2.calculateRightWallCollision();
			}
			if (player2.upWallCollisionDetector()) {
				player2.calculateUpWallCollision();
			}
			if (player2.downWallCollisionDetector(height)) {
				player2.calculateDownWallCollision();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				player1.leftButtonPressed();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				player1.rightButtonPressed();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (player1.downWallCollisionDetector(height) || (player1.playerCollisionDetector(player2) && player2.downWallCollisionDetector(height))) {
					player1.jumpButtonPressed();
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player2.leftButtonPressed();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player2.rightButtonPressed();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				if (player2.downWallCollisionDetector(height) || (player2.playerCollisionDetector(player1) && player1.downWallCollisionDetector(height))) {
					player2.jumpButtonPressed();
				}
			}

			if (ball.playerCollisionDetector(player1)) {
				ball.calculatePlayerCollision(player1);
			}
			if (ball.playerCollisionDetector(player2)) {
				ball.calculatePlayerCollision(player2);
			}

			if (player1.playerCollisionDetector(player2)) {
				player1.calculatePlayerCollision(player2);
			}

			sf::Time dt = clock.restart();

			ball.move(dt.asSeconds(), width, height);
			player1.move(dt.asSeconds(), width, height);
			player2.move(dt.asSeconds(), width, height);

			window.draw(ball.getShape());
			window.draw(player1.getShape());
			window.draw(player2.getShape());

			window.display();
		}
	}
	void Game::end() {
	}
}