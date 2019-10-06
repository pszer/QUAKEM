#include "Sound.hpp"

#include "Game.hpp"

namespace Sound {

std::vector<SoundInstance> PlayingSounds;

void Init() {
	Mix_ChannelFinished(ChannelFinished);
}

void PlaySound(const std::string& fname, double volume, void (*update)(int,int), int reg) {
	Mix_Chunk * chunk = Media.GetChunk(fname);
	if (chunk == nullptr) return;

	int ch = Mix_PlayChannel(-1, chunk, 0);
	if (ch == -1) return;

	Mix_Volume(ch, static_cast<int>(volume * MIX_MAX_VOLUME));
	PlayingSounds.emplace_back(chunk, ch, update, reg);
}

void Update() {
	for (auto s = PlayingSounds.begin(); s != PlayingSounds.end();) {
		if (s->del) {
			Mix_UnregisterAllEffects(s->channel);
			PlayingSounds.erase(s);
		} else {
			if (s->update)
				s->update(s->channel, s->reg);
			++s;
		}
	}
}

// hook function used by SDL2_Mixer
void ChannelFinished(int channel) {
	for (auto s = PlayingSounds.begin(); s != PlayingSounds.end(); ++s) {
		if (s->channel == channel) {
			PlayingSounds.erase(s);
			return;
		}
	}
}

void _ent_pos_update(int ch, int id) {
	Vec2 pos;
	decltype(Game.Entities.begin()) e;
	for (e = Game.Entities.begin(); e != Game.Entities.end(); ++e) {
		if (e->get() == nullptr) return;
		if ((*e)->UNIQUE_ID == id) {
			pos = (*e)->pos;
			break;
		}
	}

	if (e == Game.Entities.end()) return;

	pos = pos - Game.camera.GetPos();
	int angle = static_cast<int>(std::atan2(pos.y, pos.x) * 57.2957795131) + 90;
	if (angle < 0.0) angle += 360.0;
	double dist = std::sqrt(pos.x*pos.x + pos.y*pos.y);
	// limit dist from 0-255
	// new dist
	//
	// 255|         ......
	//    |    .....
	//    |  ..
	//    | .
	//    |.
	//   0|---------------- dist ->
	//
	dist = 255.0 - 255.0*255.0/(dist + 255.0);
	//std::cout << angle << " " << dist << std::endl;
	Mix_SetPosition(ch, angle, (int)dist);
}

};
