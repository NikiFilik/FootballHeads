#include "Game.hpp"

int main() {
	srand(time(0));
	nf::Game game(1600, 900, "SportHeads Football");
	game.setup();
	game.lifeCycle();
}