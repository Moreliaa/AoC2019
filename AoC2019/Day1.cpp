#include "stdafx.h"
#include "FileReader.cpp";

class Day1 {
	static long calcFuel(long mass) {
		long result = (mass / 3) - 2; // implicit floor of mass / 3
		return result >= 0 ? result : 0;
	}

	static long calcFuel(const string mass) {
		if (mass.compare("") == 0)
			return 0;
		return calcFuel(stoi(mass, nullptr, 10));
	}

	static long calcFuel_Pt2(const string mass) {
		long result = 0;
		long lastVal = calcFuel(mass);
		while (lastVal > 0) {
			result += lastVal;
			lastVal = calcFuel(lastVal);
		}
		return result;
	}

public:
	static void run() {
		vector<string> input = FileReader::readFile("input/Day1.txt");
		long result = 0;
		for each (string line in input)
		{
			result += calcFuel(line);
		}
		cout << "Total fuel (Part 1): " << result << endl;

		result = 0;
		for each (string line in input)
		{
			result += calcFuel_Pt2(line);
		}
		cout << "Total fuel (Part 2): " << result << endl;
	}
};

