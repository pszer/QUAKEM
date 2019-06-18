#include "Core.hpp"
#include "Media.hpp"

struct Media Media;

int Media::LoadMedia() {
	int textures_loaded;
	auto img_files = FilesInSubdirs(".", {"img"}); 
	textures_loaded = LoadTextures(img_files);
	Log::Log(std::to_string(textures_loaded) + " texture(s) loaded");

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

bool CheckExtension(const std::string& f, const std::vector<std::string>& exts) {
	std::filesystem::path path(f);
	std::string ext = path.extension();
	TO_LOWER(ext);

	for (auto i = exts.begin(); i != exts.end(); ++i)
		if (*i == ext) return true;
	return false;
}

int Media::LoadTextures(const std::vector<std::string>& files) {
	int count = 0;
	for (auto f : files) {
		if (!CheckExtension(f, IMG_EXTS)) continue;

		SDL_Texture* t = LoadTexture(f);
		if (t == nullptr) {
			Log::Error("Error loading textures \"" + f + "\"");
		} else {
			std::string clean = CleanFilename(f);
			textures[clean] = t;
			Log::Log("Texture \"" + clean + "\" loaded");
			++count;
		}
	}
	return count;
}

int LoadFonts(const std::vector<std::string>& files) {
	int count = 0;
	for (auto f : files) {
		if (!CheckExtension(f, TTF_EXTS)) continue;
		
		// todo
	}
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

SDL_Texture * Media::GetTexture(const std::string& str) {
	auto f = textures.find(str);
	if (f == textures.end()) return nullptr;
	return f->second;
}

Font * Media::GetFont(const std::string& str) {
	auto f = fonts.find(str);
	if (f == fonts.end()) return nullptr;
	return f->second;
}
