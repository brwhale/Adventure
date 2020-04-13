// Adventure by Garrett Skelton
//
// written on android march 2020
#include "Adventure.hpp"

int main() {
	GameInterpereter GI;
	
	GI.add(Rock(vec2(3)));
	GI.add(Rock(vec2(4)));
	GI.add(Rock(vec2(3,4)));
	GI.add(Rock(vec2(3,5)));

	GI.add(Rock(vec2(-5,-3), vec2(4, 2)));

	int range = 60;
	int offs = range/2;
	int count = 40;
	while(count-- > 0) {
		GI.add(Rock(vec2(offs-(rand()%range),offs-(rand()%range))));
	}

	GI.add(Monster(vec2(0,-8)));
	offs = 10;
	range = 30;
	for (int l = 1; l < 10; l++){
		count = 10;
		offs = 10*(l-1)*l;
		while(count-- > 0) {
			auto pos = vec2(
					offs+(rand()%range),
					offs+(rand()%range));
			if (rand()%2) pos.x *= -1;
			if (rand()%2) pos.y *= -1;
			GI.add(Monster(pos, l));
		}
	}
	
	GI.startSession();

	return 0;
}
