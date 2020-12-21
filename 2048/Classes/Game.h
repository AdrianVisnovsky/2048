// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <ctime>

#include <ncurses.h>

namespace Game2048 {

	enum Direction {
		UP = KEY_UP,
		RIGHT = KEY_RIGHT,
		DOWN = KEY_DOWN,
		LEFT = KEY_LEFT
	};

	enum MenuOption {
		NEW_GAME, HIGH_SCORE, QUIT
	};

	class Game {

	public:

		Game();
		virtual ~Game();

		/// <summary>
		/// Inits ncurses library
		/// </summary>
		void Init() const;

		void InitColors() const;

		MenuOption Menu();

		/// <summary>
		/// Prints board on screen
		/// </summary>
		void PrintGame() const;

		void PrintHighScore();

		/// <summary>
		/// Adds random tile to game board
		/// </summary>
		void AddRandomTile();

		/// <summary>
		/// Based on direction move tiles on board
		/// </summary>
		/// <param name="direction"></param>
		void MoveBoard(const Direction direction);

		void ClearScreen() const;

		void ClearBoard();

		bool IsMovePossible() const;

		bool IsRowSuitable(const int8_t rowIndex) const;

		bool IsGameWon() const;

		std::vector<uint32_t> GetHighScoresFromFile();

		bool WriteHighScoreToFile(const std::vector<uint32_t> highScores);

	private:

		// Size of board (x, y)
		const int8_t BoardSize = 4;

		const uint16_t MaxValueTile = 16384;

		// Game board
		uint16_t Board[4][4] {
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		};

		// Player score
		uint32_t Score = 0;
		
		const std::vector<std::string> GameName {
" ad888888b,    ,a888a,            a8    ad88888ba",
"d8\"     \"88  ,8P\"' `\"Y8,        ,d88   d8\"     \"8b",
"         88 ,8P       Y8,      a8P88   88       88",
"        d8P 88         88    ,d8\" 88   Y8a     a8P",
"      a8P   88         88   a8P'  88    \"Y8aaa8P\"",
"    ,d8P    88         88 ,d8\"    88    ,d8\"\"\"8b,",
"   ,d8P'    88         88 888888888888 d8\"     \"8b",
" ,d8P'      `8b       d8'         88   88       88",
"a88\"         `8ba, ,ad8'          88   Y8a     a8P",
"88888888888    \"Y888P\"            88    \"Y88888P\""
};

		const std::vector<std::string> MenuOptions {
			"New game",
			"High score",
			"Quit"
		};

		const std::string PlayerGuide = "Guide: ↑, →, ↓, ←, q - quit, r, n - new game / restart game";
		const std::string CopyrightInfo = "Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)";
		const std::string HighScoreFile = "score.txt";
		const std::string HighScoreHeader = "High score table";

		void PrintLogo() const;
		void PrintTile(const uint8_t row, const uint8_t col, const uint16_t value) const;

		MenuOption MenuWindow();

	public:

			
	};

};

