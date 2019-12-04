#include "stdafx.h"
#include <regex>

class Day4 {
	static bool checkPw(string str, bool strict) {
		int count = 0;
		int lastInt;
		bool foundDbl = false;
		for (unsigned i = 0; i < str.length(); i++)
		{
			int a = (int)str[i];
			if (i == 0)
				lastInt = a;
			if (a < lastInt) {
				return false;
			}
			else if (lastInt == a) {
				count++;
			}
			else {
				if (strict ? count == 2 : count >= 2)
					foundDbl = true;
				count = 1;
				lastInt = a;
			}
		}
		if (strict ? count == 2 : count >= 2)
			foundDbl = true;
		return foundDbl;
	}

	static bool isPw(int num, bool strict) {
		if (num < 100000 || num > 999999)
			return 0;
		string str = to_string(num);
		return checkPw(str, strict);
	}

	

public:
	static void run() {
		int lBound = 130254;
		int uBound = 678275;

		int pwCount = 0;
		int pwCount2 = 0;
		for (int i = lBound; i <= uBound; i++)
		{
			if (isPw(i, false))
				pwCount++;
			if (isPw(i, true)) {
				pwCount2++;
			}
		}
		cout << "Pt1: " << pwCount << endl;
		cout << "Pt2: " << pwCount2 << endl;
	}
};