#include "stdafx.h"
#include "Utilities.cpp"

class Day16 {

	static vector<int> calcPhase(vector<int> input) {
		vector<int> output;
		int currentRepeats;
		int repeatIdx;
		int patternIdx;
		const int pattern[4] = { 0, 1, 0, -1 };
		
		for (auto digit = 0; digit < input.size(); digit++) {
			int acc = 0;
			currentRepeats = digit + 1;
			repeatIdx = (currentRepeats == 1) ? 0 : 1;
			patternIdx = (currentRepeats == 1) ? 1 : 0;
			for (auto idx = 0; idx < input.size(); idx++) {
				acc += input[idx] * pattern[patternIdx];
				repeatIdx++;
				if (repeatIdx == currentRepeats) {
					patternIdx++;
					repeatIdx = 0;
					if (patternIdx > 3)
						patternIdx = 0;
				}
			}
			acc = abs(acc);
			acc = acc % 10;
			output.push_back(acc);
		}

		return output;
	}
	
	
public:
	static void run() {
		vector<string> input = Utilities::readFile("input/Day16.txt");
		vector<int> input_i;
		for (auto i = 0; i < input[0].size(); i++) {
			char a(input[0].at(i));
			int b = atoi(&a);
			input_i.push_back(b);
		}

		int counter = 0;
		while (counter < 100) {
			input_i = calcPhase(input_i);
			counter++;
		}
		cout << "Result: ";
		for (auto i = 0; i < 8; i++) {
			cout << input_i[i];
		}
		cout << endl;

	}
};