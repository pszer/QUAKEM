#include "Core.hpp"

#include <iostream>

int main(int argc, char ** argv) {
	std::cout << ArgIsNum(Argument(10ll)) << std::endl;
	std::cout << ArgIsNumRange(Argument(30ll), 0.0, 20.0) << std::endl;
	std::cout << ArgIsNum(Argument(ARG_STRING, "100")) << std::endl;
	std::cout << ArgIsNum(Argument(ARG_STRING, "100p")) << std::endl;
	std::vector<std::string> test_strs = { "foo", "bar", };
	std::cout << ArgIsStringElement(Argument(ARG_STRING, "foo"), test_strs) << std::endl;
	std::cout << ArgIsStringElement(Argument(ARG_STRING, "bad"), test_strs) << std::endl;

	CVARS["cvar"] = Argument(Argument(10ll));
	std::cout << ArgIsNum(Argument(ARG_CVAR, "cvar")) << std::endl;
	std::cout << ArgIsNum(Argument(ARG_CVAR, "bad_cvar")) << std::endl;
	
	return 0;

	Config::DefaultValues();

	if (Core.Init()) {
		Log::Error("QUITTING");
		Core.Quit();
		return -1;
	} else {
		Core.MainLoop();
		Config::WriteConfig("config");
		Core.Quit();
		return 0;
	}
}
