#include "stdafx.h"
#include "IntcodeC.cpp"
#include "Utilities.cpp"

class Day5 {
	
public:
	static void run() {
		vector<string> fileContent = Utilities::readFile("input/Day5.txt");
		vector<string> inputAsString = Utilities::splitString(fileContent.at(0), ",");
		vector<int> input;
		for each (string line in inputAsString) {
			input.push_back(stoi(line, nullptr, 10));
		}
		IntcodeC::runProgram(input);
	}
};