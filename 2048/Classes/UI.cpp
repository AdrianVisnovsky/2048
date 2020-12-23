// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "UI.h"
#include "HighScore.h"
#include "Game.h"

#include <ncurses.h>

namespace Game2048 {

	void PrintTile(const uint8_t row, const uint8_t col, const uint16_t value) {

		int color = GetExponent(value);

		attron(COLOR_PAIR(color));
		mvprintw(row, col, "/--------\\");
		mvprintw(row + 1, col, "|        |");

		if( value > 0 ) {
			mvprintw(row + 2, col, "| %5d  |", value);
		} else {
			mvprintw(row + 2, col, "|        |", value);
		}

		mvprintw(row + 3, col, "|        |");
		mvprintw(row + 4, col, "\\--------/");
		attroff(COLOR_PAIR(color));

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

			wattron(highScoreWin, COLOR_PAIR(30));
			mvwprintw(highScoreWin, 2, (windowHeight / 2) - HighScoreHeader.size() / 2, "%s", HighScoreHeader.c_str());
			wattroff(highScoreWin, COLOR_PAIR(30));

			for( int8_t i = 0, len = highScores.size(); i < len; i++ ) {
				mvwprintw(highScoreWin, i + 4, (windowHeight / 2) - 6, "%2d.) %7d", i + 1, highScores.at(i));
			}

			int startingRow = highScores.size() + 6;

			for( std::size_t i = 0, len = HighScoreMenuOptions.size(); i < len; i++ ) {

				if( selectedItem == i ) wattron(highScoreWin, A_REVERSE);
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
					break;

				case 'q':

					delwin(highScoreWin);
					return;
					break;

				default:
					break;
			}

		}

	}

	void PrintGame(Game *game) {

		int rows, cols;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		int rowCenter = rows / 2;
		int colCenter = cols / 2;

		int rowStart = rowCenter - game->GetBoardSize() * 5 / 2;
		int colStart = colCenter - game->GetBoardSize() * 10 / 2 - 15;

		attron(COLOR_PAIR(30));
		mvprintw(rowStart - 2, colStart, "Score: %d", game->GetScore());
		mvprintw(rowStart - 2, colStart + (game->GetBoardSize() + 1) * 10, "%s", HighScoreHeader.c_str());
		attroff(COLOR_PAIR(30));

		for( int8_t i = 0; i < game->GetBoardSize(); i++ ) {
			for( int8_t j = 0; j < game->GetBoardSize(); j++ ) {

				uint8_t row = rowStart + i * 5;
				uint8_t col = colStart + j * 10;

				Game2048::PrintTile(row, col, game->GetBoard()[i][j]);

			}	
		}

		auto highScores = Game2048::GetHighScoresFromFile();
		for( std::size_t i = 0, len = highScores.size(); i < len && i < 10; i++ ) {
			mvprintw(rowStart, colStart + (game->GetBoardSize() + 1) * 10, "%2d.) %7d", i + 1, highScores.at(i));
			rowStart++;
		}

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

	MenuOptionSizes BoardSizes() {

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		rows -= 2;

		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		WINDOW *menuWin = newwin(7, cols - 19, rows - 12, 9);
		box(menuWin, ACS_BULLET, 0);

		refresh();
		wrefresh(menuWin);

		keypad(menuWin, true);

		int8_t selectedItem = 0;

		while( true ) {

			for( int8_t i = 0, len = SizeOptions.size(); i < len; i++ ) {

				if( i == selectedItem ) wattron(menuWin, A_REVERSE);
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
					return (MenuOptionSizes) (selectedItem + 3);
					break;

				case 'q':

					delwin(menuWin);
					return BACK;
					break;

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

		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		WINDOW *menuWin = newwin(7, cols - 19, rows - 12, 9);
		box(menuWin, ACS_BULLET, 0);

		refresh();
		wrefresh(menuWin);

		keypad(menuWin, true);

		int8_t selectedItem = 0;

		while( true ) {

			for( int8_t i = 0, len = MenuOptions.size(); i < len; i++ ) {

				if( i == selectedItem ) wattron(menuWin, A_REVERSE);
				mvwprintw(menuWin, i + 2, 4, "%s", MenuOptions.at(i).c_str());
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

					if( selectedItem >= MenuOptions.size() ) {
						selectedItem = MenuOptions.size() - 1;
					}

					break;

				case 10: // ENTER

					delwin(menuWin);
					return (MenuOption) selectedItem;
					break;

				case 'n':

					delwin(menuWin);
					return NEW_GAME;
					break;

				case 'q':

					delwin(menuWin);
					return QUIT;
					break;

				default:
					break;
			}

		}

	}

	void UIInit() {

		initscr();
		keypad(stdscr, true);
		cbreak();
		noecho();

		InitColors();
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

	void UIDeInit() {
		endwin();
	}

}