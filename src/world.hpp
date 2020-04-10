#ifndef worldHpp
#define worldHpp

#include "printfuncs.hpp"
#include "objects.hpp"
#include <vector>
using std::vector;

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
		for (int i = drawSize; i--;) {
			for (int j = drawSize; j--;) {
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

		for (int i = drawSize; i--;) {
			print(screen[i]);
		}
	}
	void movePlayer(vec2 move) {
		auto pos = player.pos + move;
		for (auto&& obj : objects) {
			if (pos.inside(obj.pos, obj.pos + obj.size)) return;
		}
		player.pos = pos;

	}
	void update() {
		// move enemies and stuff
	}
};

#endif