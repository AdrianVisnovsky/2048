// Copyright (c) 2020 Adrián Kokuľa - adriankokula.eu; License: The MIT License (MIT)

#include "HighScore.h"

#include <fstream>

namespace Game2048 {

	std::vector<uint32_t> GetHighScoresFromFile() {

		// read only stream
		std::ifstream fileStream;
		fileStream.open(HighScoreFile);

		if( !fileStream.is_open() ) return {};

		std::vector<uint32_t> highScores;

		std::string line;
		while( std::getline(fileStream, line) ) {

			try {
				highScores.push_back(static_cast<uint32_t>(stoll(line)));
			} catch( const std::invalid_argument ) {

			}

		}

		fileStream.close();

		// sort loaded scores and keep max 10
		std::sort(highScores.begin(), highScores.end(), std::greater<>());
		if( highScores.size() > 10 ) {
			highScores.resize(10);
		}

		return highScores;
	}

	bool WriteHighScoreToFile(std::vector<uint32_t> highScores) {

		// write only stream
		std::ofstream fileStream;

		// open file and clean its content
		fileStream.open(HighScoreFile, std::ofstream::out | std::ofstream::trunc);

		if( !fileStream.is_open() ) return false;

		// sort scores and save only 10 best
		std::sort(highScores.begin(), highScores.end(), std::greater<>());

		for( std::size_t i = 0, len = highScores.size(); i < len && i < 10; i++ ) {
			fileStream << highScores.at(i) << std::endl;
		}

		fileStream.close();

		return true;
	}


}