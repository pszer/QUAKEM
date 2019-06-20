#include "Log.hpp"

void Log::Log(const std::string& str) {
	std::cerr << "[LOG]: " << str << std::endl;
}

void Log::Error(const std::string& str) {
	std::cerr << "\033[1;31m[ERR]\033[0m: " << str << std::endl;
}

void Log::ErrorSDL() {
	std::cerr << "\033[1;31m[SDL]\033[0m: "<< SDL_GetError() << std::endl;
}

void Log::ErrorTTF() {
	std::cerr << "\033[1;31m[TTF]\033[0m: "<< TTF_GetError() << std::endl;
}

void Log::ErrorIMG() {
	std::cerr << "\033[1;31m[IMG]\033[0m: " << IMG_GetError() << std::endl;
}

void Log::ErrorMix() {
	std::cerr << "\033[1;31m[Mix]\033[0m: " << Mix_GetError() << std::endl;
}
