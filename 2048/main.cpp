// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "Classes/Game.h"
#include "Classes/HighScore.h"
#include "Classes/UI.h"

#include <fstream>
#include <iostream>
#include <ctime>

#include <ncurses.h>

int main(const int argc, const char ** argv) {

	srand(time(NULL));
	setlocale(LC_ALL, "");

	Game2048::UIInit();

	bool loop = true;

	while( loop ) {

		Game2048::MenuOption menuOption = Game2048::Menu();

		switch( menuOption ) {

			case Game2048::NEW_GAME:
				Game2048::PlayGame();
				break;
			case Game2048::HIGH_SCORE:
				Game2048::PrintHighScore();
				break;
			case Game2048::QUIT:
				loop = false;
				break;
			default:
				break;
		}

	}

	Game2048::UIDeInit();

	return EXIT_SUCCESS;
}