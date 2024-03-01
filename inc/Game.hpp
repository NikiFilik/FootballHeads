#pragma once
#include <SFML/Graphics.hpp>

#include <string>

//#include "Object.hpp"
#include "Ball.hpp"
#include "Player.hpp"

namespace nf {
	class Game {
	private:
		int mWidth = 1920, mHeight = 1080;
		std::string mTitle = "SportHeads Football";
		sf::RenderWindow mWindow;

		//nf::Object mField;
		nf::Ball mBall;
		nf::Player mPlayer1, mPlayer2;

		int mFieldWidth = 1920, mFieldHeight = 960;
		sf::Texture mBackgroundTexture;
		sf::Sprite mBackground;

		void processEvents(sf::Time deltaTime);

		void update(sf::Time deltaTime);
		void render();
	public:
		Game();
		void run();
	};
}