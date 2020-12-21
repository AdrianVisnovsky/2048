// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "Classes/Game.h"

void PlayGame(Game2048::Game *game);
void ShowHighScore(Game2048::Game *game);

int main(const int argc, const char ** argv) {

	setlocale(LC_ALL, "");


	Game2048::Game game;
	auto test = game.GetHighScoresFromFile();

	getch();

	/*bool loop = true;
	while( loop ) {

		Game2048::MenuOption menuOption = game.Menu();

		switch( menuOption ) {

			case Game2048::NEW_GAME:
				PlayGame(&game);
				break;
			case Game2048::HIGH_SCORE:
				ShowHighScore(&game);
				break;
			case Game2048::QUIT:
				loop = false;
				break;
			default:
				break;
		}

	}*/

	return EXIT_SUCCESS;
}

void PlayGame(Game2048::Game *game) {

	game->ClearScreen();

	bool loop = true;

	game->AddRandomTile();
	game->AddRandomTile();

	game->PrintGame();

	while( loop ) {

		int input = getch();

		switch( input ) {
			case Game2048::UP:
			case Game2048::RIGHT:
			case Game2048::DOWN:
			case Game2048::LEFT:

				game->MoveBoard((Game2048::Direction) input);
				game->AddRandomTile();

				game->PrintGame();
				loop = game->IsMovePossible();

				break;

			case 'q':
				loop = false;
				break;

			case 'r':
				game->ClearBoard();
				game->AddRandomTile();
				game->AddRandomTile();
				game->PrintGame();
				break;

			default:
				break;
		}

	}

}

void ShowHighScore(Game2048::Game *game) {

}