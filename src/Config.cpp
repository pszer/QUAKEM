#include "Config.hpp"

std::vector<std::string> Config::open_files;

#define VAL(a,b) CVARS[a]=Argument(b)
#define STRVAL(a,b) CVARS[a]=Argument(ARG_STRING,b)
void Config::DefaultValues() {
	VAL("maxfps",121ll);
	VAL("showfps",1ll);
	VAL("console",1ll);
}

int Config::ExecFile(const std::string& filename) {
	for (auto& s : Config::open_files)
		if (s == filename) return 1;

	std::ifstream file(filename);
	if (!file) return 0;

	open_files.push_back(filename);

	std::string line;
	while (!file.eof()) {
		std::getline(file, line);
		if (line.empty()) continue;
		Commands::CallCommand(line);
	}

	for (auto s = Config::open_files.begin(); s != Config::open_files.end(); ++s)
	{
		if (*s == filename) {
			Config::open_files.erase(s);
			break;
		}
	}

	return 1;
}
