#ifndef functionsHpp
#define functionsHpp

#include <functional>
using std::function;
#include <string>
using std::string;
#include <vector>
using std::vector;

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

#endif