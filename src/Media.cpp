#include <iostream>

#include "Core.hpp"
#include "Media.hpp"

struct Media Media;

int Media::LoadMedia() {
	int textures_loaded;
	auto img_files = FilesInSubdirs(".", {"img"}); 
	textures_loaded = LoadTextures(img_files);
	std::cerr << textures_loaded << " texture(s) loaded" << std::endl;

	return textures_loaded;
}

void Media::FreeMedia() {
	for (auto t : textures) {
		if (t.second != nullptr)
			SDL_DestroyTexture(t.second);
	}
	textures.clear();
}

#define TO_LOWER(s) for (auto c=s.begin();c!=s.end();++c) if(*c>='A'&&*c<='Z')*c+=32;

int Media::LoadTextures(const std::vector<std::string>& files) {
	int count = 0;
	for (auto f : files) {
		std::filesystem::path path(f);
		std::string ext = path.extension();
		TO_LOWER(ext);

		// if ext is an extension for an image, i will not equal to IMG_EXTS.end()
		auto i = IMG_EXTS.begin();
		for (; i != IMG_EXTS.end(); ++i)
			if (*i == ext) break;

		if (i == IMG_EXTS.end()) continue;
		SDL_Texture* t = LoadTexture(f);
		if (t == nullptr) {
			std::cerr << "Error loading texture \"" << f << "\"" << std::endl;
		} else {
			std::string clean = CleanFilename(f);
			textures[clean] = t;
			std::cerr << "Texture \"" << clean << "\" loaded" << std::endl;
			++count;
		}
	}
	return count;
}

SDL_Texture * Media::LoadTexture(const std::string& path) {
	SDL_Surface * surface;
	SDL_Texture * texture;

	surface = IMG_Load(path.c_str());
	if (surface == nullptr) return nullptr;

	texture = SDL_CreateTextureFromSurface(Core.renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}
