#pragma once
#include "stdafx.h"

class IntcodeC {
	static int resolveParam(vector<int> &input, vector<int> command, int param, int param_digit_idx) {
		int param_mode = command.size() >= 2 + param_digit_idx ? command[command.size() - 2 - param_digit_idx] : 0;
		switch (param_mode) {
		case 0: // position
			return input[param + param_digit_idx];
		case 1: // immediate
			return param;
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
		if (command.size() == 1) // Day2 compatibility
			opcode = command[0];
		else
			opcode = command[command.size() - 2] * 10 + command[command.size() - 1];

		switch (opcode) {
		case 1: // add
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] + input[resolveParam(input, command, idx, 2)];
			return 4;
		case 2: // mul
			input[resolveParam(input, command, idx, 3)] = input[resolveParam(input, command, idx, 1)] * input[resolveParam(input, command, idx, 2)];
			return 4;
		case 3: // input

			//input[input[param + 1]]
			return 1;
		case 4: // outputs
			return 1;
		case 99:
			return -1;
		default:
			cout << "Invalid opcode: " << opcode << endl;
			return -1;
		}
	}
public:
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

};

