#ifndef AdventureHpp
#define AdventureHpp

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <functional>
#include <map>
#include <thread>
#include <chrono>
using std::deque;
using std::map;
using std::vector;
using std::string;
using std::function;
using namespace std::string_literals;
using namespace std::chrono_literals;

inline void print(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	printf("\n");
}

inline void print(const string& str) {
	printf("%s", str.c_str());
	printf("\n");
}

struct vec2 { 
	int x;
	int y;
	vec2() : x(0), y(0) {}
	vec2(int x_) : x(x_), y(x_) {}
	vec2(int x_, int y_) : x(x_), y(y_) {}
	vec2(const vec2& v) {
		x = v.x;
		y = v.y;
	}
	vec2 operator + (const vec2& v) const {
		return vec2(x+v.x, y+v.y);
	}
	vec2 operator - (const vec2& v) const {
		return vec2(x-v.x, y-v.y);
	}
	vec2 operator + (const int& i) const {
		return vec2(i) + *this;
	}
	vec2 operator - (const int& i) const {
		return *this - vec2(i);
	}
	void operator += (const vec2& v) {
		x+=v.x;
		y+=v.y;
	}
	void operator -= (const vec2& v) {
		x-=v.x;
		y-=v.y;
	}
	void operator += (const int& i) {
		*this += vec2(i);
	}
	void operator -= (const int& i) {
		*this -= vec2(i);
	}
	bool inside(const vec2& min, const vec2& max) const {
		if (x < min.x) return false;
		if (x > max.x) return false;
		if (y < min.y) return false;
		if (y > max.y) return false;

		return true;
	}
};

struct GameObject {
	vec2 pos;
	vec2 size;
	char icon = ' ';
};

inline GameObject Rock (const vec2& pos_) {
	GameObject r;
	r.pos = pos_;
	r.size = vec2(1);
	r.icon = '#';
	return r;
}

class Adventurer {
public:
	vec2 pos;
	vec2 size;
	char icon = ' ';
	Adventurer (const vec2& pos_) {
		pos = pos_;
		size = vec2(1);
		icon = 'A';
	}
};

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

class Func {
public:
	function<void(vector<string>&)> func;
	string helpText;
	void operator()(vector<string>& args) {
		func(args);
	}
};

class VoidFunc {
public:
	function<void(void)> func;
	string helpText;
	void operator()() {
		func();
	}
};

class GameInterpereter {
	World world;
	map<string, Func> funcs;
	deque<VoidFunc> commandList;
	bool running;
	vector<string> split(const string& in, const string& delim) {
		vector<string> ret;
		if (in.length()==0){
			return ret;
		}
		size_t pos = 0;
		size_t lpos = 0;
		auto dlen = delim.length();
		while ((pos = in.find(delim, lpos)) != string::npos) {
			ret.push_back(in.substr(lpos, pos - lpos));
			lpos = pos + dlen;
		}
		ret.push_back(in.substr(lpos, in.length()));
		return ret;
	}
	string getl() {
		string s;
		std::getline(std::cin, s);
		return s;
	}
	void processInput(const string& in) {
		if (in.length() <= 1) return;
		auto statements = split(in, ","s);
		for (auto&& statement : statements) {
			while (statement.size() && (statement[0] == '/' || statement[0] == ' ')) {
				statement.erase(statement.begin());
			}
			auto args = split(statement, " "s);
			auto cmd = funcs.find(args[0]);
			if (cmd != funcs.end()) {
				cmd->second(args);
			} else {
				print("Invalid command, use '/help' to list available commands");
			}
		}
	}
	void addCommand(
		vector<string>& args,
		int expectArgs,
		const string& executemsg, 
		function<void(void)> fnc
		) {
		if (args.size() > expectArgs) {
			int times = 1;
			try {
				times = std::stoi(args.back());
			} catch (std::exception e){}
			while (times-- > 0) {
				commandList.push_back({fnc, executemsg});
			}
		}
	}
public:
	GameInterpereter() {
		funcs["help"] = Func{ [this](vector<string>& args) { 
			print("");
			print("");
			print("Welcome to Adventure by Garrett Skelton!");
			print("Below is a list of available commands");
			print("");
			for (auto&& func : funcs) { 
				print(func.first + " - " + func.second.helpText); 
			} 
			print("");
		}, "Displays available commands" };
		funcs["quit"] = Func{[this](vector<string>& args){
			running = false;
		}, "quit the game"};

		funcs["wait"]= Func{[this](vector<string>& args){
			addCommand(args, 1, "waiting", [this](){
				// waiting is an empty command
			});
		}, "advance game time"};
		funcs["up"]= Func{[this](vector<string>& args){
			addCommand(args, 1, "moving up", [this](){
				world.movePlayer(vec2(0,1));
			});
		}, "move up"};
		funcs["down"]= Func{[this](vector<string>& args){
			addCommand(args, 1, "moving down", [this](){
				world.movePlayer(vec2(0,-1));
			});
		}, "move down"};
		funcs["left"]= Func{[this](vector<string>& args){
			addCommand(args, 1, "moving left", [this](){
				world.movePlayer(vec2(-1,0));
			});
		}, "move left"};
		funcs["right"]= Func{[this](vector<string>& args){
			addCommand(args, 1, "moving right", [this](){
				world.movePlayer(vec2(1,0));
			});
		}, "move right"};
	}
	void add(const GameObject& obj) {
		world.add(obj);
	}
	void startSession() {
		running = true;
		print("Hello traveler!");
		world.printView();
		while(running) {
			auto in = getl();
			processInput(in);

			while (commandList.size()) {
				print(commandList.front().helpText);
				commandList.front()();
				commandList.pop_front();

				world.update();
				world.printView();

				if (commandList.size()) {
					std::this_thread::sleep_for(.5s);
				}
			}

			print("Commands executed! Enter next move");
		}
	}
};

#endif
