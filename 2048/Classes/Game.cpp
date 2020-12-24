// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "Game.h"
#include "HighScore.h"
#include "UI.h"

#include <fstream>

namespace Game2048 {

	Game::Game(const int8_t BoardSize) {

		this->BoardSize = BoardSize;
		this->Board = std::vector(BoardSize, std::vector<uint16_t>(BoardSize));

	}

	Game::Game() : Game(4) {}

	void Game::AddRandomTile() {

		if( !IsPossibleToAddTile() ) return;

		int8_t row = row = rand() % BoardSize;
		int8_t col = row = rand() % BoardSize;

		while( !IsRowSuitable(row) ) {
			row = rand() % BoardSize;
		}

		while( Board[row][col] != 0 ) {
			col = rand() % BoardSize;
		}

		Board[row][col] = rand() % 2 == 0 ? 2 : 4;

	}

	void Game::MoveBoard(const Direction direction) {

		switch( direction ) {

			// merge tiles
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

				// move them up
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

				} else { // move them down

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

			// merge tiles
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

				// move them right
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

				} else { // move them left

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

		for( int8_t row = 0; row < BoardSize; row++ ) {
			for( int8_t col = 0; col < BoardSize; col++ ) {

				uint16_t value = Board[row][col];

				if( value == 0 ) return true;

				if( row > 0 && Board[row - 1][col] == value ) return true; // check current tile with tile above
				if( row < BoardSize - 1 && Board[row + 1][col] == value ) return true; // check current tile with tile under

				if( col > 0 && Board[row][col - 1] == value ) return true; // check current tile with tile on the left
				if( col < BoardSize - 1 && Board[row][col + 1] == value ) return true; // check current tile with on the right

			}
		}

		return false;
	}

	bool Game::IsMovePossible(const Direction direction) const {

		switch( direction ) {

			case Direction::UP:

				for( int8_t row = BoardSize - 1; row > 0; row-- ) {
					for( int8_t col = 0; col < BoardSize; col++ ) {

						if( Board[row][col] > 0 ) {
							 if( Board[row - 1][col] == 0 || Board[row][col] == Board[row - 1][col] ) return true;
						}

					}
				}

				break;

			case Direction::RIGHT:

				for( int8_t col = 0; col < BoardSize - 1; col++ ) {
					for( int8_t row = 0; row < BoardSize; row++ ) {

						if( Board[row][col] > 0 ) {
							if( Board[row][col + 1] == 0 || Board[row][col] == Board[row][col + 1] ) return true;
						}

					}
				}

				break;

			case Direction::DOWN:

				for( int8_t row = 0; row < BoardSize - 1; row++ ) {
					for( int8_t col = 0; col < BoardSize; col++ ) {

						if( Board[row][col] > 0 ) {
							if( Board[row + 1][col] == 0 || Board[row][col] == Board[row + 1][col] ) return true;
						}

					}
				}

				break;

			case Direction::LEFT:

				for( int8_t col = BoardSize - 1; col > 0; col-- ) {
					for( int8_t row = 0; row < BoardSize; row++ ) {

						if( Board[row][col] > 0 ) {
							if( Board[row][col - 1] == 0 || Board[row][col] == Board[row][col - 1] ) return true;
						}

					}
				}

				break;

			default:
				break;

		}

		return false;
	}

	bool Game::IsPossibleToAddTile() const {

		for( int8_t row = 0; row < BoardSize; row++ ) {
			for( int8_t col = 0; col < BoardSize; col++ ) {
				if( Board[row][col] == 0 ) return true;
			}
		}

		return false;
	}

	bool Game::IsRowSuitable(const int8_t rowIndex) const {

		for( int8_t col = 0; col < BoardSize; col++ ) {
			if( Board[rowIndex][col] == 0 ) {
				return true;
			}
		}

		return false;
	}

	void Game::ClearBoard() {

		for( int8_t row = 0; row < BoardSize; row++ ) {
			for( int8_t col = 0; col < BoardSize; col++ ) {
				Board[row][col] = 0;
			}
		}

		Score = 0;
	}

	void Game::StartGame() {

		ClearBoard();
		AddRandomTile();
		AddRandomTile();

	}

	uint32_t Game::GetScore() const {
		return this->Score;
	}

	int8_t Game::GetBoardSize() const {
		return this->BoardSize;
	}

	std::vector<std::vector<uint16_t>> Game::GetBoard() {
		return this->Board;
	}

}