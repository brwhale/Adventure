#ifndef objectsHpp
#define objectsHpp
#include "vec2.hpp"

class GameObject {
public:
	vec2 pos;
	vec2 size;
	char icon = ' ';
};

class StaticObject : public GameObject {
public:
	StaticObject () {}
	StaticObject (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'X';
	}
};

class Rock : public StaticObject {
public:
	Rock (const vec2& pos_, vec2 size_ = vec2(1)) {
		pos = pos_;
		size = size_;
		icon = '#';
	}
};

class Adventurer : public GameObject {
public:
	Adventurer (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'A';
	}
};

class Monster : public GameObject {
public:
	Monster (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'M';
	}
	vec2 wander(const vec2& tpos){
		auto diff = tpos - pos;
		diff.normalize();
		return diff;
	}
};

#endif
