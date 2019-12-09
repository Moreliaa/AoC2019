#include "stdafx.h"
#include "IntcodeC.cpp"

class Day2 {
	static long long runProgram(map<long long, long long> &input, int noun, int verb) {
		input[1] = noun;
		input[2] = verb;

		IntcodeC::runProgram(input, cin, cout);
		return input[0];
	}

public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day2.txt");
		{
			auto input(input_cache);
			cout << "1202 program alarm: " << runProgram(input, 12, 2) << endl;
		}

		int noun = 0;
		int verb = 0;
		long long target = 19690720;
		long long result = 0;
		while (noun <= 99)
		{
			while (verb <= 99) {
				auto input(input_cache);
				result = runProgram(input, noun, verb);
				if (result == target) {
					cout << "Noun: " << noun << " Verb: " << verb << " Result: " << 100 * noun + verb << endl;
					break;
				}
				verb++;
			}
			if (result == target) {
				break;
			}
			noun++;
			verb = 0;
		}
		if (result != target)
			cout << "Couldn't find a solution for target: " << target << endl;
	}
};

