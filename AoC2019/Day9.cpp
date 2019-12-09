#include "stdafx.h"
#include "IntcodeC.cpp"

class Day9 {
	
public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day9.txt");
		IntcodeC::runProgram(input_cache, cin, cout);
	}
};