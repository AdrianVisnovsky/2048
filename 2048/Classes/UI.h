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

	enum MenuOptionSizes {
		SMALL = 3, // 3x3
		MEDIUM, // 4x4
		BIG, // 5x5
		BACK
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

	void UIInit();

	void UIDeInit();

	void InitColors();

	void PrintLogo();

	void ClearScreen();

	void PrintTile(const uint8_t row, const uint8_t col, const uint16_t value);

	void PrintHighScore();

	void PrintGame(WINDOW *gameWindow, WINDOW *highScoreWindow, Game *game);

	void PrintGameOver();

	int GetExponent(int16_t value);

	MenuOptionSizes BoardSizes();

	MenuOption Menu();

}