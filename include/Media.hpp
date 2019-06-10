#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <map>

#include "Filesys.hpp"

extern struct Media {
	std::map<std::string, SDL_Texture*> textures;

	int LoadMedia(); // returns amount of files loaded
	void FreeMedia();

	// returns amount of textures loaded
	int LoadTextures(const std::vector<std::string>& files);
} Media;
