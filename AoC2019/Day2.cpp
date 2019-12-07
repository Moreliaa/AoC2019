#include "stdafx.h"
#include "Utilities.cpp"
#include "IntcodeC.cpp"

class Day2 {

public:
	static void run() {
		vector<string> fileContent = Utilities::readFile("input/Day2.txt");
		vector<string> inputAsString = Utilities::splitString(fileContent.at(0), ",");
		vector<int> input_cache;
		for each (string line in inputAsString) {
			input_cache.push_back(stoi(line, nullptr, 10));
		}
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

