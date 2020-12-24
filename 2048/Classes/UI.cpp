// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "UI.h"
#include "HighScore.h"
#include "Game.h"

#include <ncurses.h>

namespace Game2048 {

	void UIInit() {

		initscr();
		keypad(stdscr, true);
		cbreak();
		noecho();

		InitColors();
	}

	void UIDeInit() {
		endwin();
	}

	void InitColors() {

		if( has_colors() ) {

			start_color();

			init_pair(1, COLOR_BLACK, COLOR_RED);
			init_pair(2, COLOR_BLACK, COLOR_GREEN);
			init_pair(3, COLOR_BLACK, COLOR_YELLOW);
			init_pair(4, COLOR_BLACK, COLOR_BLUE);
			init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
			init_pair(6, COLOR_BLACK, COLOR_CYAN);
			init_pair(7, COLOR_BLACK, COLOR_WHITE);
			init_pair(8, COLOR_RED, COLOR_BLACK);
			init_pair(9, COLOR_RED, COLOR_GREEN);
			init_pair(10, COLOR_RED, COLOR_YELLOW);
			init_pair(11, COLOR_RED, COLOR_BLUE);
			init_pair(12, COLOR_RED, COLOR_MAGENTA);
			init_pair(13, COLOR_RED, COLOR_CYAN);
			init_pair(14, COLOR_RED, COLOR_WHITE);

			init_pair(30, COLOR_GREEN, COLOR_BLACK);

		}

	}

	void PlayGame() {

		int8_t boardSize = Game2048::BoardSizes();
		if( boardSize <= 0 ) return;

		Game2048::ClearScreen();

		int rows, cols;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		int rowCenter = rows / 2;
		int colCenter = cols / 2;

		int windowHeight = Game2048::TileHeight * (boardSize + 1);
		int windowWidth = Game2048::TileWidth * boardSize + 6;

		int rowStart = rowCenter - windowHeight / 2;
		int colStart = colCenter - boardSize * 10 / 2 - 15;

		// create window for tiles
		WINDOW *gameWindow = newwin(windowHeight, windowWidth, rowStart, colStart);
		box(gameWindow, ACS_BULLET, ACS_BULLET);

		// create high score window
		WINDOW *highScoreWindow = newwin(windowHeight, 22, rowStart, colStart + windowWidth + 5);
		box(highScoreWindow, ACS_VLINE, ACS_HLINE);

		refresh();
		wrefresh(gameWindow);
		wrefresh(highScoreWindow);

		std::vector<uint32_t> highScores = Game2048::GetHighScoresFromFile();

		// create game class
		Game2048::Game game(boardSize);
		game.StartGame();

		Game2048::PrintGame(gameWindow, highScoreWindow, &game, &highScores);

		bool loop = true;

		// game loop
		while( loop ) {

			int input = getch();

			switch( input ) {
				case Game2048::UP:
				case Game2048::RIGHT:
				case Game2048::DOWN:
				case Game2048::LEFT:

					if( game.IsMovePossible(static_cast<Game2048::Direction>(input)) ) {

						game.MoveBoard(static_cast<Game2048::Direction>(input));
						game.AddRandomTile();

						loop = game.IsMovePossible();

						Game2048::PrintGame(gameWindow, highScoreWindow, &game, &highScores);

						if( !loop ) {
							Game2048::PrintGameOver();
							input = getch();

							if( input == 'r' ) {

								loop = true;

								if( game.GetScore() > 0 ) {
									highScores.push_back(game.GetScore());
								}

								Game2048::WriteHighScoreToFile(highScores);
								wclear(gameWindow);
								wclear(highScoreWindow);
								box(gameWindow, ACS_BULLET, ACS_BULLET);
								box(highScoreWindow, ACS_VLINE, ACS_HLINE);

								// restarts game
								game.StartGame();

								Game2048::PrintGame(gameWindow, highScoreWindow, &game, &highScores);

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
					wclear(gameWindow);
					wclear(highScoreWindow);
					box(gameWindow, ACS_BULLET, ACS_BULLET);
					box(highScoreWindow, ACS_VLINE, ACS_HLINE);

					// restarts game
					game.StartGame();

					Game2048::PrintGame(gameWindow, highScoreWindow, &game, &highScores);

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

	void PrintTile(WINDOW* gameWindow, const uint8_t row, const uint8_t col, const uint16_t value) {

		int color = GetExponent(value);

		wattron(gameWindow, COLOR_PAIR(color));
		mvwprintw(gameWindow, row, col, "/--------\\");
		mvwprintw(gameWindow, row + 1, col, "|        |");

		if( value > 0 ) {
			mvwprintw(gameWindow, row + 2, col, "| %5d  |", value);
		} else {
			mvwprintw(gameWindow, row + 2, col, "|        |", value);
		}

		mvwprintw(gameWindow, row + 3, col, "|        |");
		mvwprintw(gameWindow, row + 4, col, "\\--------/");
		wattroff(gameWindow, COLOR_PAIR(color));

	}

	void PrintGameOver() {

		int cols = getmaxx(stdscr);
		int colCenter = cols / 2;

		attron(COLOR_PAIR(1));
		mvprintw(2, colCenter - GameOver.size() / 2, "%s", GameOver.c_str());
		attroff(COLOR_PAIR(1));

	}

	int GetExponent(int16_t value) {

		int count = 0;
		while( value > 1 ) {
			value /= 2;
			count++;
		}

		return count;
	}

	void ClearScreen() {

		clear();
		/*
		*	Parameters:
		*		Type of left border
		*		Type of right border
		*		Type of top border
		*		Type of bottom border
		*		Type of top left point edge
		*		Type of top right point edge
		*		Type of bottom left point edge
		*		Type of bottom right point edge
		*/
		border(ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_BULLET, ACS_BULLET, ACS_BULLET, ACS_BULLET);

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		int colCenter = cols / 2;

		attron(COLOR_PAIR(30));
		mvprintw(rows - 3, colCenter - PlayerGuide.size() / 2, "%s", PlayerGuide.c_str());
		mvprintw(rows - 2, colCenter - CopyrightInfo.size() / 2, "%s", CopyrightInfo.c_str());
		attroff(COLOR_PAIR(30));

	}

	void PrintHighScore() {

		ClearScreen();

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		int windowHeight = colCenter + colCenter / 2;
		int windowWidth = rowCenter + rowCenter / 2;

		WINDOW *highScoreWin = newwin(windowWidth, windowHeight, rowCenter - (windowWidth / 2), colCenter - (windowHeight / 2));
		box(highScoreWin, ACS_BULLET, 0);

		refresh();
		wrefresh(highScoreWin);

		keypad(highScoreWin, true);

		auto highScores = GetHighScoresFromFile();

		int8_t selectedItem = 0;

		while( true ) {

			// print high score
			wattron(highScoreWin, COLOR_PAIR(30));
			mvwprintw(highScoreWin, 2, (windowHeight / 2) - HighScoreHeader.size() / 2, "%s", HighScoreHeader.c_str());
			wattroff(highScoreWin, COLOR_PAIR(30));

			for( int8_t i = 0, len = highScores.size(); i < len; i++ ) {
				mvwprintw(highScoreWin, i + 4, (windowHeight / 2) - 6, "%2d.) %7d", i + 1, highScores.at(i));
			}

			int startingRow = highScores.size() + 6;

			// print choosable items
			for( std::size_t i = 0, len = HighScoreMenuOptions.size(); i < len; i++ ) {

				if( selectedItem == i ) wattron(highScoreWin, A_REVERSE); // higlight selected item
				mvwprintw(highScoreWin, startingRow + i, (windowHeight / 2) - HighScoreMenuOptions.at(i).size() / 2, "%s", HighScoreMenuOptions.at(i).c_str());
				if( selectedItem == i ) wattroff(highScoreWin, A_REVERSE);

			}

			int menuInput = wgetch(highScoreWin);

			switch( menuInput ) {

				case KEY_UP:
					selectedItem--;

					if( selectedItem < 0 ) {
						selectedItem = 0;
					}

					break;

				case KEY_DOWN:
					selectedItem++;

					if( selectedItem >= HighScoreMenuOptions.size() ) {
						selectedItem = HighScoreMenuOptions.size() - 1;
					}

					break;

				case 10: // ENTER

					if( selectedItem == 0 ) {
						WriteHighScoreToFile({});
					}

					delwin(highScoreWin);
					return;

				case 'q':

					delwin(highScoreWin);
					return;

				default:
					break;
			}

		}

	}

	void PrintGame(WINDOW *gameWindow, WINDOW *highScoreWindow, Game *game, std::vector<uint32_t> *highScores) {

		int rowStart = 3;
		int colStart = 3;

		wattron(gameWindow, COLOR_PAIR(30));
		mvwprintw(gameWindow, rowStart - 1, colStart, "Score: %d", game->GetScore());
		wattroff(gameWindow, COLOR_PAIR(30));

		// print every tile of board
		for( int8_t i = 0; i < game->GetBoardSize(); i++ ) {
			for( int8_t j = 0; j < game->GetBoardSize(); j++ ) {

				// calculate position of tile
				uint8_t row = rowStart + i * 5;
				uint8_t col = colStart + j * 10;

				Game2048::PrintTile(gameWindow, row, col, game->GetBoard()[i][j]);

			}	
		}

		wattron(highScoreWindow, COLOR_PAIR(30));
		mvwprintw(highScoreWindow, rowStart - 1, colStart, "%s", HighScoreHeader.c_str());
		wattroff(highScoreWindow, COLOR_PAIR(30));


		// sort high score just in case
		std::sort(highScores->begin(), highScores->end(), std::greater<>());

		// print high score
		for( std::size_t i = 0, len = highScores->size(); i < len && i < 10; i++ ) {
			mvwprintw(highScoreWindow, rowStart, colStart, "%2d.) %7d", i + 1, highScores->at(i));
			rowStart++;
		}

		wrefresh(gameWindow);
		wrefresh(highScoreWindow);

	}

	void PrintLogo() {

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		int rowToPrint = rowCenter - (int) GameName.size();

		attron(COLOR_PAIR(30));
		for( std::string line : GameName ) {

			int colPos = colCenter - (line.size() / 2);
			if( line.size() % 2 == 0 ) {
				colPos++;
			}

			mvprintw(rowToPrint, colPos, "%s", line.c_str());
			rowToPrint++;

		}
		attroff(COLOR_PAIR(30));

	}

	int8_t BoardSizes() {

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		WINDOW *menuWin = newwin(7, cols - 19, rows - 12, 9);
		box(menuWin, ACS_BULLET, 0);

		refresh();
		wrefresh(menuWin);

		keypad(menuWin, true);

		int8_t selectedItem = 0;

		// printing board sizes
		while( true ) {

			for( int8_t i = 0, len = SizeOptions.size(); i < len; i++ ) {

				if( i == selectedItem ) wattron(menuWin, A_REVERSE); // higliht selected item
				mvwprintw(menuWin, i + 2, 4, "%s", SizeOptions.at(i).c_str());
				if( i == selectedItem ) wattroff(menuWin, A_REVERSE);

			}

			int menuInput = wgetch(menuWin);

			switch( menuInput ) {

				case KEY_UP:
					selectedItem--;

					if( selectedItem < 0 ) {
						selectedItem = 0;
					}

					break;

				case KEY_DOWN:
					selectedItem++;

					if( selectedItem >= SizeOptions.size() ) {
						selectedItem = SizeOptions.size() - 1;
					}

					break;

				case 10: // ENTER

					delwin(menuWin);
					return selectedItem + 3;

				case 'q':

					delwin(menuWin);
					return 0;

				default:
					break;
			}

		}

	}

	MenuOption Menu() {

		ClearScreen();
		PrintLogo();
		
		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		WINDOW *menuWin = newwin(7, cols - 19, rows - 12, 9);
		box(menuWin, ACS_BULLET, 0);

		refresh();
		wrefresh(menuWin);

		keypad(menuWin, true);

		int8_t selectedItem = 0;

		// printing menu items
		while( true ) {

			for( int8_t i = 0, len = MenuOptions.size(); i < len; i++ ) {

				if( i == selectedItem ) wattron(menuWin, A_REVERSE); // highlight selected item
				mvwprintw(menuWin, i + 2, 4, "%s", MenuOptions.at(i).c_str());
				if( i == selectedItem ) wattroff(menuWin, A_REVERSE);

			}

			int menuInput = wgetch(menuWin);

			// handling user input in menu
			switch( menuInput ) {

				case KEY_UP:
					selectedItem--;

					if( selectedItem < 0 ) {
						selectedItem = 0;
					}

					break;

				case KEY_DOWN:
					selectedItem++;

					if( selectedItem >= MenuOptions.size() ) {
						selectedItem = MenuOptions.size() - 1;
					}

					break;

				case 10: // ENTER

					delwin(menuWin);
					return (MenuOption) selectedItem;

				case 'n':

					delwin(menuWin);
					return NEW_GAME;

				case 'q':

					delwin(menuWin);
					return QUIT;

				default:
					break;
			}

		}

	}


}