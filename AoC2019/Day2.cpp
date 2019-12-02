#include "stdafx.h"
#include "Utilities.cpp"

class Day2 {
	static int handleOpcode(vector<int> &input, int idx) {
		int opcode = input[idx];
		switch (opcode) {
		case 1:
			input[input[idx + 3]] = input[input[idx + 1]] + input[input[idx + 2]];
			return 4;
		case 2:
			input[input[idx + 3]] = input[input[idx + 1]] * input[input[idx + 2]];
			return 4;
		case 99:
			return -1;
		default:
			cout << "Invalid opcode: " << opcode << endl;
			return -1;
		}
	}

	static int runProgram(vector<int> &input, int noun, int verb) {
		input[1] = noun;
		input[2] = verb;

		int idx = 0;
		int incr = 0;
		do {
			incr = handleOpcode(input, idx);
			idx += incr;
		} while (incr != -1);
		return input[0];
	}

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
			cout << "1202 program alarm: " << runProgram(input, 12, 2) << endl;
		}

		int noun = 0;
		int verb = 0;
		int target = 19690720;
		int result = 0;
		while (noun <= 99)
		{
			while (verb <= 99) {
				vector<int> input(input_cache);
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

