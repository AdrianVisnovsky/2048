// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#include "Game.h"

namespace Game2048 {

	enum MenuOption {
		NEW_GAME, HIGH_SCORE, QUIT
	};

	const std::string PlayerGuide = "Guide: ↑, →, ↓, ←, q - quit/back, r - restart game, n - new game";
	const std::string CopyrightInfo = "Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)";
	const std::string HighScoreHeader = "High score table";
	const std::string GameOver = "Game over. No other move is possible!";

	const uint8_t TileWidth = 10;
	const uint8_t TileHeight = 5;

	const std::vector<std::string> MenuOptions {
		"New game",
		"High score",
		"Quit"
	};

	const std::vector<std::string> SizeOptions {
		"3x3",
		"4x4",
		"5x5"
	};

	const std::vector<std::string> HighScoreMenuOptions {
		"Clear high score table",
		"Back"
	};

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

	/// <summary>
	/// Init ncurses library
	/// </summary>
	void UIInit();

	/// <summary>
	/// DeInit ncurses library
	/// </summary>
	void UIDeInit();

	/// <summary>
	/// Init all used colors if 
	/// </summary>
	void InitColors();

	/// <summary>
	/// Game loop
	/// </summary>
	void PlayGame();

	/// <summary>
	/// Prints game logo to stdscr
	/// </summary>
	void PrintLogo();

	/// <summary>
	/// Clears stdscr, adds border and guide with copyright info
	/// </summary>
	void ClearScreen();

	/// <summary>
	/// Prints actuall state of game and high score table
	/// </summary>
	void PrintGame(WINDOW *gameWindow, WINDOW *highScoreWindow, Game *game, std::vector<uint32_t> *highScores);

	/// <summary>
	/// Print given tile to specific window
	/// </summary>
	void PrintTile(WINDOW *gameWindow, const uint8_t row, const uint8_t col, const uint16_t value);

	/// <summary>
	/// Prints table of high score
	/// </summary>
	void PrintHighScore();

	/// <summary>
	/// Prints game over label to stdscr
	/// </summary>
	void PrintGameOver();

	/// <summary>
	/// Get exponent of given number
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	int GetExponent(int16_t value);

	/// <summary>
	/// Prints predefined board sizes, keeps user choose board size 
	/// </summary>
	int8_t BoardSizes();

	/// <summary>
	/// Prints menu to user with ability to select next action
	/// </summary>
	MenuOption Menu();

}