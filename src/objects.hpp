#ifndef objectsHpp
#define objectsHpp
#include "vec2.hpp"

struct GameObject {
	vec2 pos;
	vec2 size;
	char icon = ' ';
};

inline GameObject Rock (const vec2& pos_) {
	GameObject r;
	r.pos = pos_;
	r.size = vec2(1);
	r.icon = '#';
	return r;
}

class Adventurer {
public:
	vec2 pos;
	vec2 size;
	char icon = ' ';
	Adventurer (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'A';
	}
};

#endif