#ifndef worldHpp
#define worldHpp

#include "printfuncs.hpp"
#include "objects.hpp"
#include <vector>
using std::vector;

const int viewRange = 10;
const vec2 drawSize = vec2(
	viewRange * 4 + 1, 
	viewRange * 2 + 1
);

enum class Gstate {
	overworld,
	combat,
	dialog
};

class World {
	Adventurer player = Adventurer(vec2(0));
	vector<StaticObject> objects;
	vector<Monster> monsters;
	LivingObject* combatUnit;
	bool playerHasInitiative;

	void draw(
			const GameObject& obj,
		 	vector<string>& screen, 
			const vec2& vmin, 
			const vec2& vmax) {
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
	Gstate gamestate = Gstate::overworld;
	void add(const StaticObject& obj) {
		objects.push_back(obj);
	}
	void add(const Monster& obj) {
		monsters.push_back(obj);
	}
	void printUI(){
		print("hp: %i/%i  strength: %i  armor: %i",
				player.health,
				player.maxhealth,
				player.strength,
				player.armor);
		print("enter command");
	}
	void printOverworld() {
		const vec2 vr(viewRange*2, viewRange);
		vec2 vmin = player.pos - vr;
		vec2 vmax = player.pos + vr;
		vector<string> screen(drawSize.y, string(drawSize.x, ' '));
		
		for (auto&& obj : objects) {
			draw(obj, screen, vmin, vmax);
		}
		for (auto&& obj : monsters) {
			draw(obj, screen, vmin, vmax);
		}
		screen[vr.y][vr.x] = player.icon;

		for (int i = drawSize.y; i--;) {
			print(screen[i]);
		}
	}
	void printCombat(){
		print("Combat!");
		print("enemy hp: %i/%i  strength: %i  armor: %i",
				combatUnit->health,
				combatUnit->maxhealth,
				combatUnit->strength,
				combatUnit->armor);
	}
	void printView(){
		switch (gamestate) {
			case Gstate::overworld :
				printOverworld();
				break;
			case Gstate::combat :
				printCombat();
				break;
			default: break;
		}	
	}
	void leaveCombat() {
		if (gamestate == Gstate::combat) {
			gamestate = Gstate::overworld;
		}
	}
	void attack(
			LivingObject& attacker, 
			LivingObject& defender) {
		auto damage = attacker.strength 
			- defender.armor;
		defender.health -= damage;
		print("%s took %i damage", 
				defender.name.c_str(), damage);
		if (defender.health <= 0){
			print("and died");
			leaveCombat();
		}
	}
	void attack(){
		attack(player, *combatUnit);
	}
	void startCombat(
			LivingObject& attacker, 
			LivingObject& defender){
		print("combat start");
		gamestate = Gstate::combat;
		if (attacker.otype == Otype::player) {
			playerHasInitiative = true;
			combatUnit = &defender;
		} else {
			playerHasInitiative = false;
			combatUnit = &attacker;
		}
	}
	void moveUnit(LivingObject& pbj, vec2 move) {
		const vec2 so(1);
		auto pos = pbj.pos + move;
		for (auto&& obj : objects) {
			if (pos.inside(obj.pos, 
						obj.pos + obj.size - so)) return;
		}
		for (auto&& obj : monsters) {
			if (pos.inside(obj.pos, 
						obj.pos + obj.size - so)) {
				// start combat with initiative 
				// in player favor
				startCombat(pbj, obj);
				return;
			}
		}
		if (pos.inside(player.pos, 
					player.pos + player.size - so)) {
			// start combat with initiative 
			// in monster favor
			startCombat(pbj, player);
			return;
		}

		pbj.pos = pos;
	}
	void movePlayer(vec2 move){
		moveUnit(player, move);
	}
	void updateOverworld() {
		// move enemies and stuff
		for (auto&& obj : monsters){
			moveUnit(obj, obj.wander(player.pos));
		}
	}
	void updateCombat(){
		attack(*combatUnit, player);
	}
	void update(){
		switch (gamestate) {
			case Gstate::overworld :
				updateOverworld();
				break;
			case Gstate::combat :
				updateCombat();
				break;
			default: break;
		}
	}
};

#endif
