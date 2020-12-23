// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

#include <ncurses.h>

namespace Game2048 {

	enum Direction {
		UP = KEY_UP,
		RIGHT = KEY_RIGHT,
		DOWN = KEY_DOWN,
		LEFT = KEY_LEFT
	};

	class Game {

	public:

		Game();
		Game(const int8_t BoardSize);

		/// <summary>
		/// Adds random tile to game board
		/// </summary>
		void AddRandomTile();

		/// <summary>
		/// Based on direction move tiles on board
		/// </summary>
		/// <param name="direction"></param>
		void MoveBoard(const Direction direction);

		void ClearBoard();

		bool IsMovePossible() const;

		bool IsGameWon() const;
		
		uint32_t GetScore() const;

		int8_t GetBoardSize() const;
		
		std::vector<std::vector<uint16_t>> GetBoard();

	private:

		// Size of board (x, y)
		int8_t BoardSize;

		// Player score
		uint32_t Score = 0;

		const uint16_t MaxValueTile = 16384;

		// Game board
		std::vector<std::vector<uint16_t>> Board;
		
		bool IsRowSuitable(const int8_t rowIndex) const;
		bool IsPossibleToAddTile() const;

	public:

			
	};

};

