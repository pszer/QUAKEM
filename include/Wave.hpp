#pragma once

#include <iostream>
#include <fstream>

#include "Commands.hpp"
#include "Timer.hpp"

namespace Wave {

	void LoadWave(const std::string& filename);
	void Update();

	// current delay set in ms
	extern int Delay;

	extern int Pos;
	extern std::vector<Command> wave;

};
