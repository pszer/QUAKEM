#include "Render.hpp"
#include "Media.hpp"

struct Media Media;

int Media::LoadMedia() {
	int textures_loaded;
	auto img_files = FilesInSubdirs(".", {"img"}); 
	textures_loaded = LoadTextures(img_files);
	Log::Log(std::to_string(textures_loaded) + " texture(s) loaded");

	int fonts_loaded;
	auto font_files = FilesInSubdirs(".", {"fnt"});
	fonts_loaded = LoadFonts(font_files);
	Log::Log(std::to_string(fonts_loaded) + " font(s) loaded");

	int chunks_loaded;
	auto sfx_files = FilesInSubdirs(".", {"sfx"});
	chunks_loaded = LoadChunks(sfx_files);
	Log::Log(std::to_string(chunks_loaded) + " chunk(s) loaded");

	return textures_loaded + fonts_loaded + chunks_loaded;
}

void Media::FreeMedia() {
	for (auto t : textures) {
		if (t.second != nullptr)
			SDL_DestroyTexture(t.second);
	}
	textures.clear();

	for (auto f : fonts) {
		delete f.second;
	}

	for (auto c : chunks) {
		Mix_FreeChunk(c.second);
	}
}

#define TO_LOWER(s) for (auto c=s.begin();c!=s.end();++c) if(*c>='A'&&*c<='Z')*c+=32;

bool Media::CheckExtension(const std::string& f, const std::vector<std::string>& exts) {
	std::filesystem::path path(f);
	std::string ext = path.extension();
	TO_LOWER(ext);

	for (auto i = exts.begin(); i != exts.end(); ++i) {
		if (*i == ext) {
			return true;
		}
	}
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

int Media::LoadFonts(const std::vector<std::string>& files) {
	int count = 0;
	for (auto f : files) {
		if (!CheckExtension(f, FNT_EXTS)) continue;

		Font * font = LoadFont(f);
		if (font == nullptr) {
			Log::Error("Error loading font \"" + f + "\"");
		} else {
			std::string clean = CleanFilename(f);
			fonts[clean] = font;
			Log::Log("Font \"" + clean + "\" loaded");
			++count;
		}
	}
	return count;
}

int Media::LoadChunks(const std::vector<std::string>& files) {
	int count = 0;
	for (auto f : files) {
		if (!CheckExtension(f, SFX_EXTS)) continue;

		auto chunk = LoadChunk(f);
		if (chunk == nullptr) {
				Log::Error("Error loading chunk \"" + f + "\"");
		} else {
			std::string clean = CleanFilename(f);
			chunks[clean] = chunk;
			Log::Log("SFX chunk \"" + clean +"\" loaded");
			++count;
		}
	}
	return count;
}

// LOAD FUNCTIONS
//
SDL_Texture * Media::LoadTexture(const std::string& path) {
	SDL_Surface * surface;
	SDL_Texture * texture;

	surface = IMG_Load(path.c_str());
	if (surface == nullptr) {
		Log::ErrorIMG();
		return nullptr;
	}

	texture = SDL_CreateTextureFromSurface(Renderer.renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

Font * Media::LoadFont(const std::string& path) {
	std::filesystem::path p(path);
	std::string ext = p.extension();
	TO_LOWER(ext);
	if (ext == ".ttf") {
		TTF_Font ** ttfs = new TTF_Font*[5];

		const int sizes[5] = {10,12,16,32,48};
		for (int s = 0; s < 5; ++s) {
			ttfs[s] = TTF_OpenFont(path.c_str(), sizes[s]);
			if (ttfs[s] == nullptr) {
				Log::ErrorTTF();
				delete[] ttfs;
				return nullptr;
			}
		}

		Font * font = new Font(ttfs);
		return font;
	} else if (ext == ".png") {
		SDL_Texture * glyph = LoadTexture(path);
		if (glyph == nullptr) return nullptr;

		Font * font = new Font(glyph);
		return font;
	}
	return nullptr;
}

Mix_Chunk * Media::LoadChunk(const std::string& path) {
	Mix_Chunk * chunk = Mix_LoadWAV(path.c_str());
	if (chunk == nullptr) {
		Log::ErrorMix();
		return nullptr;
	}
	return chunk;
}


// GET FUNCTIONS
//
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

Mix_Chunk * Media::GetChunk(const std::string& str) {
	auto f = chunks.find(str);
	if (f == chunks.end()) return nullptr;
	return f->second;
}
