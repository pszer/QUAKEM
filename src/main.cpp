#include "Core.hpp"

#include <iostream>

int main(int argc, char ** argv) {
	if (Core.Init()) {
		std::cerr << "ERROR: " << Core.err_msg << std::endl;
		Core.Quit();
		return -1;
	} else {
		Core.MainLoop();
		Core.Quit();
		return 0;
	}
}
