#pragma once
#include "stdafx.h"

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

	static int handleOpcode(vector<int> &input, int idx) {
		vector<int> command = extractCommand(input[idx]);
		int opcode;
		if (command.size() == 1)
			opcode = command[0];
		else
			opcode = command[command.size() - 2] * 10 + command[command.size() - 1];

		switch (opcode) {
		case 1: // add
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] + input[resolveParam(input, command, idx, 2)];
			return idx + 4;
		case 2: // mul
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] * input[resolveParam(input, command, idx, 2)];
			return idx + 4;
		case 3: // input
			int userInput;
			cout << "Input: ";
			cin >> userInput;
			input[resolveParam(input, command, idx, 1)] = userInput;
			return idx + 2;
		case 4: // outputs
			cout << "Outputs: " << input[resolveParam(input, command, idx, 1)] << endl;
			return idx + 2;
		case 5: // jump-if-true
			return input[resolveParam(input, command, idx, 1)] != 0 ? input[resolveParam(input, command, idx, 2)] : idx + 3;
		case 6: // jump-if-false
			return input[resolveParam(input, command, idx, 1)] == 0 ? input[resolveParam(input, command, idx, 2)] : idx + 3;
		case 7: // less than
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] < input[resolveParam(input, command, idx, 2)] ? 1 : 0;
			return idx + 4;
		case 8: // equals
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] == input[resolveParam(input, command, idx, 2)] ? 1 : 0;
			return idx + 4;
		case 99:
			return -1;
		default:
			cout << "Invalid opcode: " << opcode << endl;
			return -1;
		}
	}
public:
	static void runProgram(vector<int> &input) {
		int idx = 0;
		do {
			idx = handleOpcode(input, idx);
		} while (idx != -1);
	}

	static int runProgram(vector<int> &input, int noun, int verb) {
		input[1] = noun;
		input[2] = verb;

		runProgram(input);
		return input[0];
	}

};

