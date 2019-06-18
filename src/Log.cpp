#include "Log.hpp"

void Log::Log(const std::string& str) {
	std::cerr << "[LOG]: " << str << std::endl;
}

void Log::Error(const std::string& str) {
	std::cerr << "\033[1;31m[ERR]\033[0m: " << str << std::endl;
}

void Log::ErrorSDL() {
	std::cerr << "[SDL]: " << SDL_GetError() << std::endl;
}

void Log::ErrorTTF() {
	std::cerr << "[TTF]: " << TTF_GetError() << std::endl;
}

void Log::ErrorIMG() {
	std::cerr << "[IMG]: " << IMG_GetError() << std::endl;
}
