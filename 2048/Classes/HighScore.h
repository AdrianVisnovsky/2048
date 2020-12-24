// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#pragma once

#include <vector>
#include <cinttypes>
#include <string>

namespace Game2048 {

	const std::string HighScoreFile = "score.txt";

	/// <summary>
	/// Get top 10 scores from high score file
	/// </summary>
	/// <returns></returns>
	std::vector<uint32_t> GetHighScoresFromFile();

	/// <summary>
	/// Save top 10 scores from high score file
	/// </summary>
	/// <param name="highScores"></param>
	/// <returns></returns>
	bool WriteHighScoreToFile(std::vector<uint32_t> highScores);

}