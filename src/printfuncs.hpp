#ifndef printfuncsHpp
#define printfuncsHpp

#include <string>
using std::string;
using namespace std::string_literals;
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <chrono>
using namespace std::chrono_literals;
#include <thread>

namespace Color { 
	const string Esc = "\033";
	const string Default = "[0m";
	const char* Clear() {
		return (Esc + Default).c_str();
	}
	
	const string Mode_Italic = "[3;";
	const string Mode_Light = "[2;";
	const string Mode_Bold = "[1;";
	const string Mode_Default = "[";

	const string BG_Black = "40";
	const string BG_DarkRed = "41";
	const string BG_Green = "42";
	const string BG_Brown = "43";
	const string BG_Blue = "44";
	const string BG_Pink = "45";
	const string BG_Teal = "46";
	const string BG_LightGrey = "47";
	const string BG_Grey = "100";
	const string BG_Red = "101";
	const string BG_LightGreen = "102";
	const string BG_Yellow = "103";
	const string BG_LightBlue = "104";
	const string BG_LightPink = "105";
	const string BG_LightTeal = "106";
	const string BG_White = "107";

	const string Black = "30m";
	const string DarkRed = "31m";
	const string Green = "32m";
	const string Brown = "33m";
	const string Blue = "34m";
	const string Pink = "35m";
	const string Teal = "36m";
	const string LightGrey = "37m";
	const string Grey = "90m";
	const string Red = "91m";
	const string LightGreen = "92m";
	const string Yellow = "93m";
	const string LightBlue = "94m";
	const string LightPink = "95m";
	const string LightTeal = "96m";
	const string White = "97m";	

	string GetStr(
		const string& mode, 
		const string& bg, 
		const string& col) {
		return (
			// clear previous color
			Esc + Default
			// set new color
			+ Esc + mode 
			+ ";" + bg + ";" + col
		);
	}
	string GetStr(
		const string& bg, 
		const string& col) {
		return GetStr(Mode_Default, bg, col);
	}
	string GetStr(const string& col) {
		return GetStr(Mode_Default, BG_Black, col);
	}
	const char* Get(
		const string& mode, 
		const string& bg, 
		const string& col) {
		return GetStr(mode, bg, col).c_str();
	}
	const char* Get(
		const string& bg, 
		const string& col) {
		return GetStr(bg, col).c_str();
	}
	const char* Get(const string& col) {
		return GetStr(col).c_str();
	}
}

inline void print(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
	printf("\e[K\n");
}

inline void print(const string& str) {
	printf("%s\e[K\n", str.c_str());
}

inline void nap(){
	std::this_thread::sleep_for(.16s);
}

inline string getl() {
	string s;
	std::getline(std::cin, s);
	return s;
}

#endif
