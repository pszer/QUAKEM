#include "Config.hpp"

#define VAL(a,b) CVARS[a]=Argument(b)
#define STRVAL(a,b) CVARS[a]=Argument(ARG_STRING,b)
void Config::DefaultValues() {
	VAL("maxfps",121ll);
	VAL("showfps",1ll);
	VAL("console",1ll);
}

int Config::ExecFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) return 0;

	std::string line;
	while (!file.eof()) {
		std::getline(file, line);
		if (line.empty()) continue;
		Commands::CallCommand(line);
	}

	return 1;
}
