#pragma once

#include <iostream>
#include <fstream>

#include "Commands.hpp"

#define CONFIG_PATH "config"

namespace Config {

	// config values are stored in CVARs

	// maxfps 121
	void DefaultValues();

	// returns 1 on success, 0 on error
	int ExecFile(const std::string& filename);

};
