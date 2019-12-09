#pragma once
#include "stdafx.h"
#include "Utilities.cpp"
#include <map>

class IntcodeProgram {
public:
	map<long long, long long> *input;
	long long idx;
	long long relative_base;

	IntcodeProgram() {
		this->idx = 0;
		this->relative_base = 0;
	}

	IntcodeProgram(map<long long, long long> &input) {
		this->input = &input;
		this->idx = 0;
		this->relative_base = 0;
	}
};

class IntcodeC {
	static long long getValue(map<long long, long long> &input, long long idx) {
		auto search = input.find(idx);
		if (search == input.end()) {
			input[idx] = 0;
		}
		return input[idx];
	}

	static long long resolveParam(IntcodeProgram &prog, vector<int> &command, long long param, unsigned digit_idx) {
		auto &input = *prog.input;
		int param_mode = command.size() >= 2 + digit_idx ? command[command.size() - 2 - digit_idx] : 0;
		long long param_idx = param + digit_idx;
		switch (param_mode) {
		case 0: // position
			return getValue(input, param_idx);
		case 1: // immediate
			return param_idx;
		case 2: // relative
			return getValue(input, param_idx) + prog.relative_base;
		default:
			cout << "Invalid param_mode: " << param_mode << endl;
			return -1;
		}
	}
	static vector<int> extractCommand(long long input) {
		vector<int> digits;
		auto it = digits.begin();
		long long modop = 1;
		long long next_modop = modop * 10;
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

	static long long peekOpcode(IntcodeProgram &prog) {
		auto &input = *prog.input;
		long long &idx = prog.idx;
		auto command = extractCommand(getValue(input, idx));
		int opcode;
		if (command.size() == 1)
			opcode = command[0];
		else
			opcode = command[command.size() - 2] * 10 + command[command.size() - 1];
		return opcode;
	}

	static bool handleOpcode(IntcodeProgram &prog, istream &s_in, ostream &s_out, bool haltOnOutput=false) {
		auto &input = *prog.input;
		long long &idx = prog.idx;
		vector<int> command = extractCommand(input[idx]);
		int opcode;
		if (command.size() == 1)
			opcode = command[0];
		else
			opcode = command[command.size() - 2] * 10 + command[command.size() - 1];

		switch (opcode) {
		case 1: // add
			input[resolveParam(prog, command, idx, 3)] = getValue(input, resolveParam(prog, command, idx, 1)) + getValue(input, resolveParam(prog, command, idx, 2));
			idx = idx + 4;
			return false;
		case 2: // mul
			input[resolveParam(prog, command, idx, 3)] = getValue(input, resolveParam(prog, command, idx, 1)) * getValue(input, resolveParam(prog, command, idx, 2));
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
			input[resolveParam(prog, command, idx, 1)] = userInput;
			idx = idx + 2;
			return false;
		case 4: // outputs
			if (&s_out == &cout)
				s_out << "Outputs: ";
			s_out << getValue(input, resolveParam(prog, command, idx, 1)) << endl;
			idx = idx + 2;
			return haltOnOutput;
		case 5: // jump-if-true
			idx = getValue(input, resolveParam(prog, command, idx, 1)) != 0 ? getValue(input, resolveParam(prog, command, idx, 2)) : idx + 3;
			return false;
		case 6: // jump-if-false
			idx = getValue(input, resolveParam(prog, command, idx, 1)) == 0 ? getValue(input, resolveParam(prog, command, idx, 2)) : idx + 3;
			return false;
		case 7: // less than
			input[resolveParam(prog, command, idx, 3)] = getValue(input, resolveParam(prog, command, idx, 1)) < getValue(input,resolveParam(prog, command, idx, 2)) ? 1 : 0;
			idx = idx + 4;
			return false;
		case 8: // equals
			input[resolveParam(prog, command, idx, 3)] = getValue(input,resolveParam(prog, command, idx, 1)) == getValue(input,resolveParam(prog, command, idx, 2)) ? 1 : 0;
			idx = idx + 4;
			return false;
		case 9: // adjust relative base
			prog.relative_base += getValue(input, resolveParam(prog, command, idx, 1));
			idx = idx + 2;
			return false;
		case 99:
			return true;
		default:
			cout << "Invalid opcode: " << opcode << endl;
			return true;
		}
	}
public:
	static long long runProgram(IntcodeProgram &prog, istream &s_in, ostream &s_out, bool haltOnOutput=false) {
		bool halt;
		do {
			halt = handleOpcode(prog, s_in, s_out, haltOnOutput);
		} while (!halt);
		return peekOpcode(prog);
	}

	static long long runProgram(map<long long, long long> &input, istream &s_in, ostream &s_out, bool haltOnOutput=false) {
		IntcodeProgram prog(input);
		return runProgram(prog, s_in, s_out, haltOnOutput);
	}

	static map<long long, long long> getInput(string filenameOrString, bool interpretAsString=false) {	
		vector<string> fileContent;
		if (interpretAsString)
			fileContent.push_back(filenameOrString);
		else
			fileContent = Utilities::readFile(filenameOrString);
		vector<string> inputAsString = Utilities::splitString(fileContent.at(0), ",");
		map<long long, long long> input;
		long long idx = 0;
		for each (string number in inputAsString) {
			input[idx] = stoi(number, nullptr, 10);
			idx++;
		}
		return input;
	}

};

