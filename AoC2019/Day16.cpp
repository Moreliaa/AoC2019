#include "stdafx.h"
#include "Utilities.cpp"

class Day16 {

	static vector<int> calcPhase(vector<int> input) {
		vector<int> output;
		int currentRepeats;
		int repeatIdx;
		int patternIdx;
		const int pattern[4] = { 0, 1, 0, -1 };
		bool patternsLooped = false;
		
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

		auto inputCopy(input_i);

		int counter = 0;
		while (counter < 100) {
			inputCopy = calcPhase(inputCopy);
			counter++;
		}
		cout << "Result Pt1: ";
		for (auto i = 0; i < 8; i++) {
			cout << inputCopy[i];
		}
		cout << endl;

		int offset = stoi(input[0].substr(0, 7));

		cout << "Offset: " << offset << endl;

		inputCopy = vector<int>(input_i);
		int idxInList = offset % inputCopy.size();
		int factor = (inputCopy.size() * 10000 - (offset - idxInList)) / inputCopy.size();
		if ((inputCopy.size() * 10000 - (offset - idxInList)) % input[0].size() != 0)
			throw;
		vector<int> bigInput;
		bigInput.reserve(inputCopy.size() * factor + 1);
		for (int i = idxInList; i < inputCopy.size() && factor > 0; i++)
		{
			bigInput.push_back(inputCopy[i]);
			if (i == inputCopy.size() - 1) {
				factor--;
				i = -1;
			}
		}
		

		counter = 0;
		while (counter < 100) {
			long long sumOfValues = 0;
			auto it = bigInput.end();
			do {
				it--;
				int d = *it;
				sumOfValues += d;
				int newDigit = sumOfValues % 10;
				*it = newDigit;
			} while (it != bigInput.begin());
			counter++;
		}


		cout << "Result Pt2: ";
		for (auto i = 0; i < 8; i++) {
			cout << bigInput[i];
		}
		cout << endl;
	}
};