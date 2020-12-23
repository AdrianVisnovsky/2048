// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "Classes/Game.h"
#include "Classes/HighScore.h"
#include "Classes/UI.h"

#include <fstream>
#include <iostream>
#include <ctime>

void PlayGame();

int main(const int argc, const char ** argv) {

	srand(time(NULL));
	setlocale(LC_ALL, "");

	Game2048::UIInit();

	bool loop = true;

	while( loop ) {

		Game2048::MenuOption menuOption = Game2048::Menu();

		switch( menuOption ) {

			case Game2048::NEW_GAME:
				PlayGame();
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

void PlayGame() {

	Game2048::MenuOptionSizes result = Game2048::BoardSizes();
	if( result == Game2048::BACK ) return;

	Game2048::Game game((int8_t) result);

	Game2048::ClearScreen();
	game.ClearBoard();

	std::vector<uint32_t> highScores = Game2048::GetHighScoresFromFile();

	game.AddRandomTile();
	game.AddRandomTile();

	Game2048::PrintGame(&game);

	bool loop = true;

	while( loop ) {

		int input = getch();

		switch( input ) {
			case Game2048::UP:
			case Game2048::RIGHT:
			case Game2048::DOWN:
			case Game2048::LEFT:

				game.MoveBoard((Game2048::Direction) input);
				game.AddRandomTile();

				loop = game.IsMovePossible() || game.IsGameWon();

				break;

			case 'q':
				loop = false;
				break;

			case 'r':
				game.ClearBoard();
				game.AddRandomTile();
				game.AddRandomTile();
				break;

			default:
				break;

		}

		Game2048::PrintGame(&game);

	}

	if( game.GetScore() > 0 ) {
		highScores.push_back(game.GetScore());
	}

	Game2048::WriteHighScoreToFile(highScores);

}
