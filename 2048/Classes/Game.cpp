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

		if( rand() % 2 == 0 ) {
			Board[row][col] = 2;
		} else {
			Board[row][col] = 4;
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

	bool Game::IsPossibleToAddTile() const {

		for( int8_t i = 0; i < BoardSize; i++ ) {
			for( int8_t j = 0; j < BoardSize; j++ ) {
				if( Board[i][j] == 0 ) return true;
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

	void Game::ClearBoard() {

		for( int8_t i = 0; i < BoardSize; i++ ) {
			for( int8_t j = 0; j < BoardSize; j++ ) {
				Board[i][j] = 0;
			}
		}

		Score = 0;
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