#include "stdafx.h"
#include "Utilities.cpp"
#include <map>
#include <algorithm>

class Day10 {

	static string createKey(unsigned x, unsigned y) {
		return to_string(x) + "," + to_string(y);
	}

	static bool checkDir(map<string, string> &field, unsigned x, unsigned y, unsigned x1, unsigned y1, unsigned stepSizeX, unsigned stepSizeY) {
		unsigned greaterX = max(x, x1);
		unsigned lesserX = min(x, x1);
		unsigned greaterY = max(y, y1);
		unsigned lesserY = min(y, y1);
		lesserX += stepSizeX;
		lesserY += stepSizeY;

		while ((lesserX < greaterX && stepSizeX != 0) || (lesserY < greaterY && stepSizeY != 0))
		{
			if (field[createKey(lesserX, lesserY)] == "#")
				return false;
			lesserX += stepSizeX;
			lesserY += stepSizeY;
		}
		return true;
	}

	static void reduce(unsigned &greater, unsigned &lesser) {
		if (greater % lesser == 0) {
			greater /= lesser;
			lesser = 1;
		}
	}

	static bool checkLos(map<string, string> &field, unsigned x, unsigned y, unsigned x1, unsigned y1) {
			unsigned stepX = x > x1 ? x - x1 : x1 - x;
			unsigned stepY = y > y1 ? y - y1 : y1 - y;
			if (stepX == 0)
				stepY = 1;
			else if (stepY == 0)
				stepX = 1;
			else
				stepX > stepY ? reduce(stepX, stepY) : reduce(stepY, stepX);
			return checkDir(field, x, y, x1, y1, stepX, stepY);
	}

	static void mapLosForAst(map<string, string> &field, map<string, int> &losCount, unsigned width, unsigned height, unsigned x, unsigned y, string printTarget) {
		string targetKey = createKey(x, y);
		bool print = targetKey.compare(printTarget) == 0;
		for (unsigned y1 = 0; y1 < height; y1++)
		{
			for (unsigned x1 = 0; x1 < width; x1++)
			{
				if (x == x1 && y == y1) {
					if (print)
						cout << "@";
					continue;
				}
				string key = createKey(x1, y1);
				if (field[key] == "#") {
					if (checkLos(field, x, y, x1, y1)) {
						losCount[targetKey] += 1;
						if (print)
							cout << "*";
					} else if (print)
						cout << "#";
				}
				else if (print)
					cout << ".";
			}
			if (print)
				cout << endl;
		}
	}

	static void mapLos(map<string, string> &field, map<string, int> &losCount, unsigned width, unsigned height, string printTarget="") {
		for (unsigned y = 0; y < height;y++)
		{
			for (unsigned x = 0; x < width; x++)
			{
				string key = createKey(x, y);
				bool print = key.compare(printTarget) == 0;
				if (field[key] == "#") {
					if (print)
						cout << "===" << key << endl;
					mapLosForAst(field, losCount, width, height, x, y, printTarget);
					if (print)
						cout << "===" << endl;
				}
			}
		}
	}
	
public:
	static void run() {
		vector<string> fileContent = Utilities::readFile("input/Day10.txt");

		map<string, string> field;
		map<string, int> losCount;

		unsigned j = 0;
		for each(string line in fileContent) {
			if (line == "") // EOF
				continue;
			for (unsigned i = 0; i < line.length(); i++)
			{
				string key = createKey(i, j);
				field[key] = line[i];
				losCount[key] = 0;
			}
			j++;
		}
		unsigned width = fileContent[0].length();
		unsigned height = j;
		mapLos(field, losCount, width, height, "5,8");
		int maximum = 0;
		string maxKey = "";
		auto it = losCount.begin();
		do {
			int newMax = max(maximum, it->second);
			if (newMax > maximum)
				maxKey = it->first;
			maximum = newMax;
			it++;
		} while (it != losCount.end());
		cout << maxKey << " -> Max: " << maximum << endl;
	}
};