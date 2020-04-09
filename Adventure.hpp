#ifndef AdventureHpp
#define AdventureHpp

#include <stdio.h>
#include <stdarg.h>
#include <vector>

using std::vector;

inline void print(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	printf("\n");
}

struct vec2 { 
	int x;
	int y;
	vec2(){
		x = y = 0;
	}
	vec2(int x_) {
		x = x_;
		y = x_;
	}
	vec2(int x_, int y_) : x(x_), y(y_) {}
	vec2(const vec2& v) {
		x = v.x;
		y = v.y;
	}
	vec2 operator + (const vec2& v) const {
		return vec2(x+v.x, y+v.y);
	}
	vec2 operator - (const vec2& v) const {
		return vec2(x-v.x, y-v.y);
	}
	vec2 operator + (const int& i) const {
		return vec2(i) + *this;
	}
	vec2 operator - (const int& i) const {
		return *this - vec2(i);
	}
	bool inside(const vec2& min, const vec2& max) const {
		if (x < min.x) return false;
		if (x > max.x) return false;
		if (y < min.y) return false;
		if (y > max.y) return false;

		return true;
	}
};

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

class World {
	Adventurer player = Adventurer(vec2(0));
	vector<GameObject> objects;

public:
	void add(const GameObject& obj) {
		objects.push_back(obj);
	}
	void printView() {
		const int viewRange = 10;
		const int drawSize = viewRange * 2 + 1;
		vec2 vmin = player.pos - viewRange;
		vec2 vmax = player.pos + viewRange;
		char screen[drawSize][drawSize+1];
		for (int i = 0; i < drawSize; i++) {
			for (int j = 0; j < drawSize; j++) {
				screen[i][j] = ' ';
			}
			screen[i][drawSize] = 0;
		}
		for (auto&& obj : objects) {
			if (!obj.pos.inside(vmin, vmax)) {
				continue;
			}
			auto spos = obj.pos - vmin;
			screen[spos.y][spos.x] = obj.icon;
		}
		screen[viewRange][viewRange] = player.icon;

		for (int i = 0; i < drawSize; i++) {
			print(screen[i]);
		}
	}
};

#endif
