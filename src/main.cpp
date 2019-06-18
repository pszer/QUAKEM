#include "Core.hpp"

#include <iostream>

int main(int argc, char ** argv) {
	if (Core.Init()) {
		Log::Error("QUITTING");
		Core.Quit();
		return -1;
	} else {
		Core.MainLoop();
		Core.Quit();
		return 0;
	}
}
