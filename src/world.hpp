#ifndef worldHpp
#define worldHpp

#include "printfuncs.hpp"
#include "objects.hpp"
#include <vector>
using std::vector;

const int viewRange = 10;
const int drawSize = viewRange * 2 + 1;

class World {
	Adventurer player = Adventurer(vec2(0));
	vector<StaticObject> objects;
	vector<Monster> monsters;

	void draw(const GameObject& obj, vector<string>& screen, const vec2& vmin, const vec2& vmax) {
		for (auto x = obj.size.x; x--;) {
			for (auto y = obj.size.y; y--;) {
				auto pos = obj.pos + vec2(x, y);
				if (!pos.inside(vmin, vmax)) {
					continue;
				}
				auto spos = pos - vmin;
				screen[spos.y][spos.x] = obj.icon;
			}
		}
	}
public:
	void add(const StaticObject& obj) {
		objects.push_back(obj);
	}
	void add(const Monster& obj) {
		monsters.push_back(obj);
	}
	void printView() {		
		vec2 vmin = player.pos - viewRange;
		vec2 vmax = player.pos + viewRange;
		vector<string> screen(drawSize, string(drawSize, ' '));
		
		for (auto&& obj : objects) {
			draw(obj, screen, vmin, vmax);
		}
		for (auto&& obj : monsters) {
			draw(obj, screen, vmin, vmax);
		}
		screen[viewRange][viewRange] = player.icon;

		for (int i = drawSize; i--;) {
			print(screen[i]);
		}
	}
	void movePlayer(vec2 move) {
		auto pos = player.pos + move;
		for (auto&& obj : objects) {
			if (pos.inside(obj.pos, obj.pos + obj.size)) return;
		}
		for (auto&& obj : monsters) {
			if (pos.inside(obj.pos, obj.pos + obj.size)) {
				// start combat with initiative in our favor
				return;
			}
		}
		player.pos = pos;
	}
	void update() {
		// move enemies and stuff
	}
};

#endif