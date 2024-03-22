#pragma once
#include <SFML/Graphics.hpp>

#include "Ball.hpp"
#include "Player.hpp"

#include <string>

namespace nf {
	class Game {
	private:
		int mWidth = 1920, mHeight = 1080;
		std::string mTitle = "SportHeads Football";
		sf::RenderWindow mWindow;

		nf::Ball mBall;
		nf::Player mPlayer1, mPlayer2;

		int mFieldWidth = 1920, mFieldHeight = 960;
		sf::Texture mBackgroundTexture;
		sf::Sprite mBackground;

		sf::Texture mGatesTexture;
		sf::Sprite mGates;
		sf::CircleShape mCrossbar1, mCrossbar2;
		float mCrossbar1PositionX = 200.f, mCrossbar1PositionY = 660.f;
		float mCrossbar2PositionX = 1720.f, mCrossbar2PositionY = 660.f;
		float mCrossbarRadius = 10.f;

		int mScore1 = 0, mScore2 = 0;
		sf::Font mFont;
		sf::Text mTextScore1, mTextScore2, mGoalSign;
		int mGoalFlag = 0;

		void processInput();
		void update();
		void render();
	public:
		Game();
		void run();
	};
}