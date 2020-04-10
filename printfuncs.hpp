#ifndef printfuncsHpp
#define printfuncsHpp

#include <string>
using std::string;
#include <stdio.h>
#include <stdarg.h>

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

#endif