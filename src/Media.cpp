#include "Media.hpp"

struct Media Media;

int Media::LoadMedia() {
	int textures_loaded;
	auto img_files = FilesInSubdirs(".", "img"); 
	textures_loaded = LoadTextures(img_files);
}

void Media::FreeMedia() {
	for (auto t : textures) {
		if (t.second != nullptr)
			SDL_DestroyTexture(t.second);
	}
}

int Media::LoadTextures(const std::vector<std::string>& files) {
	for (auto f : files) {
		std::filesystem::path path(f);
	}
}
