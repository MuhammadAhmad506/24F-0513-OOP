#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include"GameHeader.h"

using namespace std;
int main() {
	// Seed the random number generator
	srand(static_cast<unsigned int>(time(0)));
    GameManager game;
    game.startGame();
    return 0;
}