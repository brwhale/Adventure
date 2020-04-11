#ifndef worldHpp
#define worldHpp

#include "printfuncs.hpp"
#include "objects.hpp"
#include <vector>
using std::vector;

const vec2 vr(26, 9);
const vec2 drawSize = vr * 2 + 1;

enum class Gstate {
	overworld,
	combat,
	dialog,
	death
};

class World {
	Adventurer player = Adventurer(vec2(0));
	vector<StaticObject> objects;
	vector<Monster> monsters;
	LivingObject* combatUnit;

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
		objects.back().index = objects.size() - 1;
	}
	void add(const Monster& obj) {
		monsters.push_back(obj);
		monsters.back().index = monsters.size() - 1;
	}
	void printUnitStats(const LivingObject& u){
		print("%s%s: lvl:%i gold:%i hp:%i/%i str:%i amr:%i",
			Color::Get(Color::BG_Blue, Color::White),
			u.name.c_str(),
			u.level,
			u.gold,
			u.health,
			u.maxhealth,
			u.strength,
			u.armor);
	}
	void printUI(){
		printUnitStats(player);
		print("%senter command",
			Color::Get(Color::BG_White, Color::Black));
	}
	void printOverworld() {
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
			screen[i] = Color::GetStr(Color::White) 
				+ "|" + screen[i] + "|";
			print(screen[i]);
		}
	}
	void printCombat(){
		printUnitStats(*combatUnit);
	}
	void printView(){
		switch (gamestate) {
			case Gstate::overworld:
				printOverworld();
				break;
			case Gstate::combat:
				printCombat();
				break;
			case Gstate::death:
				print("%syou are dead", 
					Color::Get(Color::BG_Red, Color::Black));
				break;
			default: break;
		}	
	}
	void leaveCombat() {
		gamestate = Gstate::overworld;
	}
	void unitDeath(
		LivingObject& attacker, 
		LivingObject& defender) {
		nap();
		print("and died");
		auto xpgain = 20 * defender.level;
		attacker.gold += defender.gold;
		attacker.xp += xpgain;
		nap();
		print("%s%s gained %i xp and %i gold",
			Color::Get(Color::BG_Brown, Color::White),
			attacker.name.c_str(),
			xpgain,
			defender.gold);
		auto newlevel = getLevel(attacker.xp);
		if (newlevel != attacker.level){
			attacker.levelUp(newlevel);
			nap();
			print("%scongratulations %s reached level %i",
				Color::Get(Color::BG_Teal, Color::Yellow),
				attacker.name.c_str(),
				newlevel);
		}
		switch (defender.otype){
		case Otype::monster: {
			auto id = defender.index;
			monsters[id] = monsters.back();
			monsters[id].index = id;
			monsters.pop_back();
			print("%s%u monsters left",
				Color::Get(Color::BG_Brown, Color::Green),
				monsters.size());
			leaveCombat();
		}
			break;
		case Otype::player:
			gamestate = Gstate::death;
			break;
		default:
			break;
		}
		print("%senter to continue",
			Color::Get(Color::BG_White, Color::Black));
		getl();
	}
	void attack(
		LivingObject& attacker, 
		LivingObject& defender) {
		nap();
		print("%s%s attacked %s",
			Color::Get(Color::BG_White, Color::Red),
			attacker.name.c_str(),
			defender.name.c_str());
		auto damage = attacker.strength 
			- defender.armor;
		defender.health -= damage;
		nap();
		print("%s%s took %i damage", 
			Color::Get(Color::BG_Red, Color::Black),
			defender.name.c_str(), damage);
		if (defender.health <= 0){
			unitDeath(attacker, defender);
		}
		nap();
	}
	void attack(){
		attack(player, *combatUnit);
	}
	void startCombat(
			LivingObject& attacker, 
			LivingObject& defender){
		
		gamestate = Gstate::combat;
		if (attacker.otype == Otype::player) {
			combatUnit = &defender;
			print("%sCombat start!",
				Color::Get(Color::BG_Green, Color::White));
		} else {
			combatUnit = &attacker;
			print("%sCombat start!",
				Color::Get(Color::BG_Red, Color::White));
		}
		attack(attacker, defender);
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
				if (pbj.otype == Otype::player) {
					startCombat(pbj, obj);
				}
				return;
			}
		}
		if (pos.inside(player.pos, 
					player.pos + player.size - so)) {
			if (pbj.otype == Otype::monster) {
				startCombat(pbj, player);
			}
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
			case Gstate::overworld:
				updateOverworld();
				break;
			case Gstate::combat:
				updateCombat();
				break;
			default: break;
		}
	}
};

#endif
