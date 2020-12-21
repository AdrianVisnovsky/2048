// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "Game.h"

#include <fstream>

namespace Game2048 {

	Game::Game() {

		srand(time(NULL));

		Init();

	}

	Game::~Game() {
		endwin();
	}

	void Game::Init() const {

		initscr();
		keypad(stdscr, true);
		cbreak();
		noecho();

		InitColors();

	}

	void Game::InitColors() const {

		if( has_colors() ) {

			start_color();

			init_pair(1, COLOR_GREEN, COLOR_BLACK);
			init_pair(2, COLOR_BLACK, COLOR_RED);
			init_pair(4, COLOR_BLACK, COLOR_GREEN);
			init_pair(8, COLOR_BLACK, COLOR_YELLOW);
			init_pair(16, COLOR_BLACK, COLOR_BLUE);
			init_pair(32, COLOR_BLACK, COLOR_MAGENTA);
			init_pair(64, COLOR_BLACK, COLOR_CYAN);
			init_pair(128, COLOR_BLACK, COLOR_WHITE);
			init_pair(256, COLOR_RED, COLOR_BLACK);
			init_pair(512, COLOR_RED, COLOR_GREEN);
			init_pair(1024, COLOR_RED, COLOR_YELLOW);
			init_pair(2048, COLOR_RED, COLOR_BLUE);
			init_pair(4096, COLOR_RED, COLOR_MAGENTA);
			init_pair(8192, COLOR_RED, COLOR_CYAN);
			init_pair(16384, COLOR_RED, COLOR_WHITE);

		}

	}

	MenuOption Game::Menu() {

		ClearScreen();
		PrintLogo();
		return MenuWindow();
	}

	void Game::PrintLogo() const {

		int cols, rows;
		getmaxyx(stdscr, rows, cols);
		
		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		int rowToPrint = rowCenter - (int) GameName.size();
		
		attron(COLOR_PAIR(1));
		for( std::string line : GameName ) {

			int colPos = colCenter - (line.size() / 2);
			if( line.size() % 2 == 0 ) {
				colPos++;
			}

			mvprintw(rowToPrint, colPos, "%s", line.c_str());
			rowToPrint++;

		}
		attroff(COLOR_PAIR(1));

	}

	MenuOption Game::MenuWindow() {

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		WINDOW *menuWin = newwin(7, cols - 19, rows - 12, 9);
		box(menuWin, 0, 0);

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
					return (MenuOption)selectedItem;
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

	void Game::AddRandomTile() {

		int8_t row = row = rand() % BoardSize;
		int8_t col = row = rand() % BoardSize;

		while( !IsRowSuitable(row) ) {
			row = rand() % BoardSize;
		}

		while( Board[row][col] != 0 ) {
			col = rand() % BoardSize;
		}

		if( rand() % 2 == 0 ) {
			Board[row][col] = 2;
		} else {
			Board[row][col] = 4;
		}

	}

	void Game::PrintGame() const {

		attron(COLOR_PAIR(1));
		mvprintw(2, 5, "Score: %d", Score);
		attroff(COLOR_PAIR(1));

		for( int8_t i = 0; i < BoardSize; i++ ) {
			for( int8_t j = 0; j < BoardSize; j++ ) {

				uint8_t row = 4 + (i * 5);
				uint8_t col = 5 + j * 10;

				PrintTile(row, col, Board[i][j]);
			}
		}

	}

	void Game::MoveBoard(const Direction direction) {

		switch( direction ) {
			case Game2048::UP:
			case Game2048::DOWN:

				for( int8_t col = 0; col < BoardSize; col++ ) {
					for( int8_t row = 0; row < BoardSize; row++ ) {

						uint16_t value = Board[row][col];
						if( value == 0 ) continue;

						for( int8_t newRow = row + 1; newRow < BoardSize; newRow++ ) {

							uint16_t secondValue = Board[newRow][col];
							if( secondValue == 0 ) continue;
							if( value != secondValue ) break;

							Board[row][col] = value * 2;
							Board[newRow][col] = 0;

							Score += Board[row][col];
							break;

						}

					}
				}

				if( direction == Game2048::UP ) {

					for( int8_t col = 0; col < BoardSize; col++ ) {
						for( int8_t row = 0; row < BoardSize; row++ ) {

							if( Board[row][col] != 0 ) continue;

							for( int8_t newRow = row + 1; newRow < BoardSize; newRow++ ) {

								if( Board[newRow][col] == 0 ) continue;

								Board[row][col] = Board[newRow][col];
								Board[newRow][col] = 0;
								break;

							}

						}
					}

				} else {

					for( int8_t col = 0; col < BoardSize; col++ ) {
						for( int8_t row = BoardSize - 1; row >= 0; row-- ) {

							if( Board[row][col] != 0 ) continue;

							for( int8_t newRow = row -1; newRow >= 0; newRow-- ) {

								if( Board[newRow][col] == 0 ) continue;

								Board[row][col] = Board[newRow][col];
								Board[newRow][col] = 0;
								break;

							}

						}
					}

				}

				break;
			case Game2048::RIGHT:
			case Game2048::LEFT:

				for( int8_t row = 0; row < BoardSize; row++ ) {
					for( int8_t col = 0; col < BoardSize; col++ ) {

						uint16_t value = Board[row][col];
						if( value == 0 ) continue;

						for( int8_t newCol = col + 1; newCol < BoardSize; newCol++ ) {

							uint16_t secondValue = Board[row][newCol];
							if( secondValue == 0 ) continue;
							if( value != secondValue ) break;

							Board[row][col] = value * 2;
							Board[row][newCol] = 0;

							Score += Board[row][col];
							break;

						}

					}
				}

				if( direction == Game2048::RIGHT ) {

					for( int8_t row = 0; row < BoardSize; row++ ) {
						for( int8_t col = BoardSize - 1; col >= 0; col-- ) {

							if( Board[row][col] != 0 ) continue;

							for( int8_t newCol = col - 1; newCol >= 0; newCol-- ) {

								if( Board[row][newCol] == 0 ) continue;

								Board[row][col] = Board[row][newCol];
								Board[row][newCol] = 0;
								break;

							}

						}
					}

				} else {

					for( int8_t row = 0; row < BoardSize; row++ ) {
						for( int8_t col = 0; col < BoardSize; col++ ) {

							if( Board[row][col] != 0 ) continue;

							for( int8_t newCol = col + 1; newCol < BoardSize; newCol++ ) {

								if( Board[row][newCol] == 0 ) continue;

								Board[row][col] = Board[row][newCol];
								Board[row][newCol] = 0;
								break;

							}

						}
					}

				}

				break;
			default:
				break;
		}

	}

	bool Game::IsMovePossible() const {

		for( int8_t i = 0; i < BoardSize; i++ ) {
			for( int8_t j = 0; j < BoardSize; j++ ) {

				uint16_t value = Board[i][j];

				if( value == 0 ) return true;

				if( i > 0 && Board[i - 1][j] == value ) return true;
				if( i < BoardSize - 1 && Board[i + 1][j] == value ) return true;

				if( j > 0 && Board[i][j - 1] == value ) return true;
				if( j < BoardSize - 1 && Board[i][j + 1] == value ) return true;

			}
		}

		return false;
	}

	bool Game::IsRowSuitable(const int8_t rowIndex) const {

		for( int8_t i = 0; i < BoardSize; i++ ) {

			if( Board[rowIndex][i] == 0 ) {
				return true;
			}

		}

		return false;
	}

	bool Game::IsGameWon() const {

		for( int8_t i = 0; i < BoardSize; i++ ) {
			for( int8_t j = 0; j < BoardSize; j++ ) {

				if( Board[i][j] == MaxValueTile ) return true;

			}
		}

		return false;
	}

	void Game::ClearScreen() const {

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

		attron(COLOR_PAIR(1));
		mvprintw(rows - 3, colCenter - PlayerGuide.size() / 2, "%s", PlayerGuide.c_str());
		mvprintw(rows - 2, colCenter - CopyrightInfo.size() / 2, "%s", CopyrightInfo.c_str());
		attroff(COLOR_PAIR(1));

	}

	void Game::PrintTile(const uint8_t row, const uint8_t col, const uint16_t value) const {

		attron(COLOR_PAIR(value));
		mvprintw(row, col,     "/--------\\");
		mvprintw(row + 1, col, "|        |");
		mvprintw(row + 2, col, "| %5d  |", value);
		mvprintw(row + 3, col, "|        |");
		mvprintw(row + 4, col, "\\--------/");
		attroff(COLOR_PAIR(value));

	}

	void Game::ClearBoard() {

		for( int8_t i = 0; i < BoardSize; i++ ) {
			for( int8_t j = 0; j < BoardSize; j++ ) {
				Board[i][j] = 0;
			}
		}

	}

	std::vector<uint32_t> Game::GetHighScoresFromFile() {

		std::ifstream fileStream;
		fileStream.open(HighScoreFile);

		if( !fileStream.is_open() ) return {};

		std::vector<uint32_t> highScores;

		std::string line;
		while( std::getline(fileStream, line) ) {

			try {
				highScores.push_back((uint32_t) stoll(line));
			} catch( const std::invalid_argument ) {

			}

		}

		fileStream.close();

		//std::sort(highScores.begin(), highScores.end());
		return highScores;
	}

	bool Game::WriteHighScoreToFile(const std::vector<uint32_t> highScores) {

		std::ofstream fileStream;
		fileStream.open(HighScoreFile, std::ofstream::out | std::ofstream::trunc);

		if( !fileStream.is_open() ) return false;

		//std::sort(highScores.begin(), highScores.end());

		for( uint32_t score : highScores ) {
			fileStream << score << std::endl;
		}

		fileStream.close();

		return true;
	}

	void Game::PrintHighScore() {

		ClearScreen();

		int cols, rows;
		getmaxyx(stdscr, rows, cols);

		int colCenter = cols / 2;
		int rowCenter = rows / 2;

		int windowHeight = colCenter;
		int windowWidth = rowCenter;

		WINDOW *highScoreWin = newwin(windowWidth, windowHeight, rowCenter - (windowWidth / 2), colCenter - (windowHeight / 2));
		box(highScoreWin, 0, 0);

		refresh();
		wrefresh(highScoreWin);

		keypad(highScoreWin, true);

		int8_t selectedItem = 0;

		auto highScores = GetHighScoresFromFile();

		while( true ) {

			mvwprintw(highScoreWin, 2, colCenter / 2 - HighScoreHeader.size() / 2, "%s", HighScoreHeader.c_str());

			for( int8_t i = 0, len = highScores.size(); i < len; i++ ) {
				mvwprintw(highScoreWin, i + 4, 4, "%d.) %d", i + 1, highScores.at(i));
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

					if( selectedItem >= MenuOptions.size() ) {
						selectedItem = MenuOptions.size() - 1;
					}

					break;

				case 10: // ENTER

					delwin(highScoreWin);
					return;
					break;

				case 'r':

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

}