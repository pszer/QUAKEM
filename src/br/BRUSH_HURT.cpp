#include "br/BRUSH_HURT.hpp"

void Brushes::Hurt::CollideFunc(Entity * ent) {
	bool found = false;
	for (auto p : timers) {
		if (p.first == ent->UNIQUE_ID) {
			found = true;

			double rate = GetValue("rate");
			if (p.second.GetSeconds() < rate) {
				return;
			}
		}
	}

	if (!found) {
		auto p = std::make_pair<int, Timer>((int)ent->UNIQUE_ID, Timer());
		p.second.Start();
		timers.push_back(p);
	}

	ent->hitpoints -= GetValue("damage"); 
}

void Brushes::Hurt::Update() {
	double rate = GetValue("rate");
	for (auto p = timers.begin(); p != timers.end();) {
		if (p->second.GetSeconds() >= rate)
			timers.erase(p);
		else ++p;
	}
}
