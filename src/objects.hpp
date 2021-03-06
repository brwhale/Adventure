#ifndef objectsHpp
#define objectsHpp
#include "vec2.hpp"

enum class Otype{
	rock,
	player,
	monster
};

inline int getLevel(int xp){
	return 1 + sqrt(xp) / 10;
}

inline int xpToLevel(int level) {
 auto a = 10 * (level - 1);
 return a*a;
}

class GameObject {
public:
	vec2 pos;
	vec2 size;
	char icon = ' ';
	Otype otype;
	size_t index;
};

class StaticObject : public GameObject {
public:
	StaticObject () {}
	StaticObject (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'X';
	}
};

class LivingObject : public GameObject {
public:
	int health;
	int maxhealth;
	int strength;
	int armor;
	int level;
	int gold;
	int xp;
	string name;
	LivingObject () {}
	void levelUp(int newLevel) {
		while (level < newLevel){
			level++;
			maxhealth += 5 * level;
			strength += level/2;
			armor += 1 + level/5;
			if (otype == Otype::player){
				print("%scongrats! %s grew to level %i",
					Color::Get(Color::BG_Teal, Color::Yellow),
					name.c_str(), level);
				print("health increased to %i", maxhealth);
				print("strength increased to %i", strength);
				print("armor increased to %i", armor);
			}
		}
		health = maxhealth;
	}
	void levelUp() {
		levelUp(level + 1);
	}
};

class Rock : public StaticObject {
public:
	Rock (const vec2& pos_, vec2 size_ = vec2(1)) {
		pos = pos_;
		size = size_;
		icon = '#';
		otype = Otype::rock;
	}
};

class Adventurer : public LivingObject {
public:
	Adventurer (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'A';
		level = 1;
		maxhealth = 50;
		health = 50;
		strength = 6;
		armor = 3;
		gold = 0;
		xp = 0;
		name = "you";
		otype = Otype::player;
	}
};

class Monster : public LivingObject {
public:
	Monster (const vec2& pos_, int level_ = 1) {
		pos = pos_;
		size = vec2(1);
		icon = '0' + level_;
		level = 1;
		maxhealth = 30;
		health = maxhealth;
		strength = 4;
		armor = 1;
		gold = strength;
		xp = 0;
		name = "monster";
		otype = Otype::monster;
		levelUp(level_);
	}
	vec2 wander(const vec2& tpos){
		auto diff = tpos - pos;
		auto len = diff.length();
		if (len < 9) {
			diff.normalize();
			return diff;
		}
		return vec2((rand()%3)-1,(rand()%3)-1);
	}
};

#endif
