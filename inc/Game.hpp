#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#include "Ball.hpp"
#include "Player.hpp"


namespace nf {
	class Game {
	private:
		int width, height;
		std::string title;
		sf::RenderWindow window;
		sf::Clock clock;
		nf::Ball ball;
		nf::Player player1, player2;
	public:
		Game(int width, int height, std::string title);
		void setup();
		void lifeCycle();
		void end();
	};
}