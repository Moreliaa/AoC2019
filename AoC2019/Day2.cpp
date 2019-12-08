#include "stdafx.h"
#include "IntcodeC.cpp"

class Day2 {

public:
	static void run() {
		vector<int> input_cache = IntcodeC::getInput("input/Day2.txt");
		{
			vector<int> input(input_cache);
			cout << "1202 program alarm: " << IntcodeC::runProgram(input, 12, 2) << endl;
		}

		int noun = 0;
		int verb = 0;
		int target = 19690720;
		int result = 0;
		while (noun <= 99)
		{
			while (verb <= 99) {
				vector<int> input(input_cache);
				result = IntcodeC::runProgram(input, noun, verb);
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

