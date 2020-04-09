// Adventure by Garrett Skelton
//
// written on android march 2020
#include "Adventure.hpp"

int main() {
	World world;
	print("Hello traveler!");
	world.add(Rock(vec2(3)));
	world.add(Rock(vec2(4)));
	world.add(Rock(vec2(3,4)));
	world.add(Rock(vec2(3,5)));

	world.printView();

	return 0;
}
