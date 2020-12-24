// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "Classes/Game.h"
#include "Classes/HighScore.h"
#include "Classes/UI.h"

#include <fstream>
#include <iostream>
#include <ctime>

#include <ncurses.h>

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

	Game2048::ClearScreen();

	Game2048::Game game((int8_t) result);

	int rows, cols;
	getmaxyx(stdscr, rows, cols);

	rows -= 2;

	int rowCenter = rows / 2;
	int colCenter = cols / 2;

	int windowHeight = Game2048::TileHeight * (game.GetBoardSize() + 1);
	int windowWidth = Game2048::TileWidth * game.GetBoardSize() + 6;

	int rowStart = rowCenter - windowHeight / 2;
	int colStart = colCenter - game.GetBoardSize() * 10 / 2 - 15;

	WINDOW *gameWindow = newwin(windowHeight, windowWidth, rowStart, colStart);
	box(gameWindow, ACS_BULLET, ACS_BULLET);

	WINDOW *highScoreWindow = newwin(windowHeight, 22, rowStart, colStart + windowWidth + 5);
	box(highScoreWindow, ACS_VLINE, ACS_HLINE);

	refresh();
	wrefresh(gameWindow);
	wrefresh(highScoreWindow);

	game.ClearBoard();

	std::vector<uint32_t> highScores = Game2048::GetHighScoresFromFile();

	game.AddRandomTile();
	game.AddRandomTile();

	Game2048::PrintGame(gameWindow, highScoreWindow, &game);

	bool loop = true;

	while( loop ) {

		int input = getch();

		switch( input ) {
			case Game2048::UP:
			case Game2048::RIGHT:
			case Game2048::DOWN:
			case Game2048::LEFT:

				if( game.IsMovePossible((Game2048::Direction) input) ) {

					game.MoveBoard((Game2048::Direction) input);
					game.AddRandomTile();

					loop = game.IsMovePossible() || game.IsGameWon();

					Game2048::PrintGame(gameWindow, highScoreWindow, &game);

					if( !loop ) {
						Game2048::PrintGameOver();
						input = getch();

						if( input == 'r' ) {

							loop = true;

							if( game.GetScore() > 0 ) {
								highScores.push_back(game.GetScore());
							}

							Game2048::WriteHighScoreToFile(highScores);
							highScores = Game2048::GetHighScoresFromFile();
							wclear(gameWindow);
							wclear(highScoreWindow);
							box(gameWindow, ACS_BULLET, ACS_BULLET);
							box(highScoreWindow, ACS_VLINE, ACS_HLINE);

							game.ClearBoard();
							game.AddRandomTile();
							game.AddRandomTile();

							Game2048::PrintGame(gameWindow, highScoreWindow, &game);

						}

					}

				}

				break;

			case 'q':
				loop = false;
				break;

			case 'r':

				if( game.GetScore() > 0 ) {
					highScores.push_back(game.GetScore());
				}

				Game2048::WriteHighScoreToFile(highScores);
				highScores = Game2048::GetHighScoresFromFile();
				wclear(gameWindow);
				wclear(highScoreWindow);
				box(gameWindow, ACS_BULLET, ACS_BULLET);
				box(highScoreWindow, ACS_VLINE, ACS_HLINE);

				game.ClearBoard();
				game.AddRandomTile();
				game.AddRandomTile();

				Game2048::PrintGame(gameWindow, highScoreWindow, &game);

				break;

			default:
				break;

		}

	}

	if( game.GetScore() > 0 ) {
		highScores.push_back(game.GetScore());
	}

	Game2048::WriteHighScoreToFile(highScores);

	delwin(gameWindow);
	delwin(highScoreWindow);

}
