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
	
	GI.startSession();

	return 0;
}
