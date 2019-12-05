#include "Wave.hpp"
	
// current delay set in ms
int Wave::Delay = 0;

int Wave::Pos = 0;
std::vector<Command> Wave::wave;

using namespace Wave;

void Wave::LoadWave(const std::string& filename) {
	Delay = 0;
	Pos   = 0;

	std::ifstream file(filename);
	if (!file) return;

	wave.clear();
	while (!file.eof()) {
		std::string str;
		Command com;

		std::getline(file, str);
		if (str.empty()) continue;

		if (Parser::ParseCommand(str, com)) {
			wave.push_back(com);
		}
	}
}

void Wave::Update() {
	if (wave.empty()) return;

	if (!Delay) {
		if (Pos == wave.size()) return;
		
		auto str = Commands::Execute(wave.at(Pos));
		if (!str.empty()) Log::Add(str);

		++Pos;
	} else {
		Delay -= FrameLimit.deltatime_n.count() / 1000000;
		if (Delay < 0) Delay = 0;
	}
}
