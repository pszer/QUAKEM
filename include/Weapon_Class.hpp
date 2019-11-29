#pragma once

#include <cmath>
#include <map>

#include "Vec2.hpp"

class Entity;

class Weapon {
public:
	Weapon(std::map<std::string, double> k, Entity * p): keys(k), parent(p) {
		if (keys.find("ammo") == keys.end())
			SetKey("ammo", -1.0);
	}

	virtual void Update() { ; }

	virtual bool Fire(Vec2 aim) = 0;
	
	void SetKey(const std::string& s, double d) {
		keys[s] = d;
	}
	double GetKey(const std::string& s) {
		auto f = keys.find(s);
		if (f == keys.end()) return 0.0;
		return f->second;
	}

	std::map<std::string, double> keys;

	Entity * parent;

	virtual ~Weapon() { }
};

#include "Entity.hpp"
