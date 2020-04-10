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

	GI.add(Monster(vec2(0,-8)));
	
	GI.startSession();

	return 0;
}
