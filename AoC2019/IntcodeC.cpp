#pragma once
#include "stdafx.h"
#include "Utilities.cpp"

class IntcodeProgram {
public:
	vector<int> *input;
	int idx;

	IntcodeProgram() {
		this->idx = 0;
	}

	IntcodeProgram(vector<int> &input) {
		this->input = &input;
		this->idx = 0;
	}
};

class IntcodeC {
	static int resolveParam(vector<int> &input, vector<int> &command, int param, unsigned digit_idx) {
		int param_mode = command.size() >= 2 + digit_idx ? command[command.size() - 2 - digit_idx] : 0;
		int param_idx = param + digit_idx;
		switch (param_mode) {
		case 0: // position
			return input[param_idx];
		case 1: // immediate
			return param_idx;
		default:
			cout << "Invalid param_mode: " << param_mode << endl;
			return -1;
		}
	}
	static vector<int> extractCommand(int input) {
		vector<int> digits;
		auto it = digits.begin();
		int modop = 1;
		int next_modop = modop * 10;
		int digit = 0;
		while (input != 0) {
			while (input % next_modop != 0) {
				input -= modop;
				digit++;
			}
			modop = next_modop;
			next_modop = modop * 10;
			it = digits.insert(it, digit);
			digit = 0;
		}
		return digits;
	}

	static int peekOpcode(IntcodeProgram &prog) {
		vector<int> &input = *prog.input;
		int &idx = prog.idx;
		vector<int> command = extractCommand(input[idx]);
		int opcode;
		if (command.size() == 1)
			opcode = command[0];
		else
			opcode = command[command.size() - 2] * 10 + command[command.size() - 1];
		return opcode;
	}

	static bool handleOpcode(IntcodeProgram &prog, istream &s_in, ostream &s_out, bool haltOnOutput=false) {
		vector<int> &input = *prog.input;
		int &idx = prog.idx;
		vector<int> command = extractCommand(input[idx]);
		int opcode;
		if (command.size() == 1)
			opcode = command[0];
		else
			opcode = command[command.size() - 2] * 10 + command[command.size() - 1];

		switch (opcode) {
		case 1: // add
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] + input[resolveParam(input, command, idx, 2)];
			idx = idx + 4;
			return false;
		case 2: // mul
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] * input[resolveParam(input, command, idx, 2)];
			idx = idx + 4;
			return false;
		case 3: // input
			int userInput;
			if (&s_in == &cin) {
				cout << "Input: ";
				s_in >> userInput;
			}
			else {
				string line;
				getline(s_in, line);
				userInput = stoi(line, nullptr, 10);
			}
			input[resolveParam(input, command, idx, 1)] = userInput;
			idx = idx + 2;
			return false;
		case 4: // outputs
			if (&s_out == &cout)
				s_out << "Outputs: ";
			s_out << input[resolveParam(input, command, idx, 1)] << endl;
			idx = idx + 2;
			return haltOnOutput;
		case 5: // jump-if-true
			idx = input[resolveParam(input, command, idx, 1)] != 0 ? input[resolveParam(input, command, idx, 2)] : idx + 3;
			return false;
		case 6: // jump-if-false
			idx = input[resolveParam(input, command, idx, 1)] == 0 ? input[resolveParam(input, command, idx, 2)] : idx + 3;
			return false;
		case 7: // less than
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] < input[resolveParam(input, command, idx, 2)] ? 1 : 0;
			idx = idx + 4;
			return false;
		case 8: // equals
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] == input[resolveParam(input, command, idx, 2)] ? 1 : 0;
			idx = idx + 4;
			return false;
		case 99:
			return true;
		default:
			cout << "Invalid opcode: " << opcode << endl;
			return true;
		}
	}
public:
	static int runProgram(IntcodeProgram &prog, istream &s_in, ostream &s_out, bool haltOnOutput=false) {
		bool halt;
		do {
			halt = handleOpcode(prog, s_in, s_out, haltOnOutput);
		} while (!halt);
		return peekOpcode(prog);
	}

	static int runProgram(vector<int> &input, istream &s_in, ostream &s_out, bool haltOnOutput=false) {
		IntcodeProgram prog(input);
		return runProgram(prog, s_in, s_out, haltOnOutput);
	}

	static int runProgram(vector<int> &input, int noun, int verb) {
		input[1] = noun;
		input[2] = verb;

		runProgram(input, cin, cout);
		return input[0];
	}

	static vector<int> getInput(string filenameOrString, bool interpretAsString=false) {	
		vector<string> fileContent;
		if (interpretAsString)
			fileContent.push_back(filenameOrString);
		else
			fileContent = Utilities::readFile(filenameOrString);
		vector<string> inputAsString = Utilities::splitString(fileContent.at(0), ",");
		vector<int> input;
		for each (string number in inputAsString) {
			input.push_back(stoi(number, nullptr, 10));
		}
		return input;
	}

};
