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
		level = newLevel;
		maxhealth += 5 * level;
		health = maxhealth;
		strength += level/2;
		armor += 1;
		print("health increased to %i", health);
		print("strength increased to %i", strength);
		print("armor increased to %i", armor);
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
		armor = 2;
		gold = 0;
		xp = 0;
		name = "you";
		otype = Otype::player;
	}
};

class Monster : public LivingObject {
public:
	Monster (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'M';
		level = 1;
		maxhealth = 30;
		health = 30;
		strength = 3;
		armor = 1;
		gold = 3 + rand()%3;
		xp = 0;
		name = "monster";
		otype = Otype::monster;
	}
	vec2 wander(const vec2& tpos){
		auto diff = tpos - pos;
		diff.normalize();
		return diff;
	}
};

#endif
