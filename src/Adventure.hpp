#ifndef AdventureHpp
#define AdventureHpp

#include <deque>
#include <map>
using std::deque;
using std::map;

#include "printfuncs.hpp"
#include "world.hpp"
#include "functions.hpp"

class GameInterpereter {
	World world;
	map<string, Func> funcs;
	deque<VoidFunc> commandList;
	VoidFunc lastCommand ={[](){},""};
	bool running;
	Gstate gamestate = Gstate::overworld;
	vector<string> split(
			const string& in, 
			const string& delim) {
		vector<string> ret;
		if (in.length()==0){
			return ret;
		}
		size_t pos = 0;
		size_t lpos = 0;
		auto dlen = delim.length();
		while ((pos = in.find(delim, lpos)) 
				!= string::npos) {
			ret.push_back(in.substr(lpos, pos - lpos));
			lpos = pos + dlen;
		}
		ret.push_back(in.substr(lpos, in.length()));
		return ret;
	}
	void processInput(const string& in) {
		if (in.length() <= 1) {
			commandList.push_back(lastCommand);
		 	return;
		}
		auto statements = split(in, ","s);
		for (auto&& statement : statements) {
			while (statement.size() && 
					(statement[0] == '/'
					 || statement[0] == ' ')) {
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
		funcs["help"] = Func{ [this](
				vector<string>& args) { 
			print("%sBelow is a list of commands:",
				Color::Get(Color::BG_Green, Color::White));
			print("");
			for (auto&& func : funcs) { 
				print(func.first + " - " 
						+ func.second.helpText); 
			} 
			print("");
		}, "Displays available commands" };
		funcs["quit"] = Func{[this](
				vector<string>& args){
			running = false;
			print("%squitting%s",
				Color::Get(Color::White),
				Color::Clear());
		}, "quit the game"};

		funcs["wait"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "waiting", [this](){
				// waiting is an empty command
			});
		}, "advance game time without taking action"};
		funcs["up"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "moving up", [this](){
				world.movePlayer(vec2(0,1));
			});
		}, "move up"};
		funcs["down"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "moving down", [this](){
				world.movePlayer(vec2(0,-1));
			});
		}, "move down"};
		funcs["left"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "moving left", [this](){
				world.movePlayer(vec2(-1,0));
			});
		}, "move left"};
		funcs["right"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "moving right", [this](){
				world.movePlayer(vec2(1,0));
			});
		}, "move right"};

		funcs["run"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "trying to run!",
					[this](){
						world.leaveCombat();
					});
		}, "run from combat"};
		funcs["fight"] = Func{[this](
				vector<string>& args){
			addCommand(args, 0, "attacking!",
					[this](){
						world.attack();
					});
		}, "fight in combat"};
		// shorthand
		funcs["w"] = funcs["wait"];
		funcs["u"] = funcs["up"];
		funcs["d"] = funcs["down"];
		funcs["l"] = funcs["left"];
		funcs["r"] = funcs["right"];
		funcs["f"] = funcs["fight"];
	}
	void add(const StaticObject& obj) {
		world.add(obj);
	}
	void add(const Monster& obj) {
		world.add(obj);
	}
	void startSession() {
		running = true;
		print("%sHello traveler!",
			Color::Get(Color::BG_Blue, Color::Yellow));
		nap();
		print("Welcome to the Other world.");
		nap();
		print("by Garrett Skelton");
		nap();
		print("press enter to step outside");
		getl();
		
		world.printView();
		world.printUI();
		while(running) {
			processInput(getl());
			while (commandList.size()) {
				print("%s%s",
					Color::Get(Color::White),
					commandList.front().helpText.c_str());
				lastCommand = commandList.front();
				commandList.front()();
				commandList.pop_front();
				world.update();
				world.printView();
				world.printUI();

				if (world.gamestate != gamestate){
					gamestate = world.gamestate;
					commandList.clear();
				}

				if (commandList.size()) {
					nap();
				}
			}
		}
	}
};

#endif
