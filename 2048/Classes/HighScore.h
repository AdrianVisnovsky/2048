// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#pragma once

#include <vector>
#include <cinttypes>
#include <string>

namespace Game2048 {

	const std::string HighScoreFile = "score.txt";

	std::vector<uint32_t> GetHighScoresFromFile();

	bool WriteHighScoreToFile(std::vector<uint32_t> highScores);

}