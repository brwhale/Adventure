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

	count = 10;
	while(count-- > 0) {
		GI.add(Monster(3 * vec2(offs-(rand()%range),offs-(rand()%range))));
	}
	
	GI.startSession();

	return 0;
}
