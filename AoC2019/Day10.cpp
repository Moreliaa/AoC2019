#include "stdafx.h"
#include "Utilities.cpp"
#include <map>
#include <algorithm>

class Day10 {
	class Point {
	public:
		int x;
		int y;

		Point() {
			this->x = 0;
			this->y = 0;
		}
		Point(int x, int y) {
			this->x = x;
			this->y = y;
		}
	};

	static string createKey(unsigned x, unsigned y) {
		return to_string(x) + "," + to_string(y);
	}

	static bool checkDir(map<string, string> &field, unsigned x1, unsigned y1, unsigned x2, unsigned y2, Point &step) {
		int xNext = x1 + step.x;
		int yNext = y1 + step.y;
		int xMin = min(x1, x2);
		int xMax = max(x1, x2);
		int yMin = min(y1, y2);
		int yMax = max(y1, y2);

		while ((xNext > xMin && xNext < xMax && step.x != 0) || (yNext > yMin && yNext < yMax && step.y != 0))
		{
			if (field[createKey(xNext, yNext)] == "#")
				return false;
			xNext = xNext + step.x;
			yNext = yNext + step.y;
		}
		return true;
	}

	static Point getStepSize(unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
		int stepX = x2 - x1;
		int stepY = y2 - y1;
		if (stepY > 1 && stepX < -1)
			int a = 0;
		if (stepX == 0)
			stepY /= abs(stepY);
		else if (stepY == 0)
			stepX /= abs(stepX);
		else {
			int abs1 = abs(stepX);
			int abs2 = abs(stepY);
			int next = max(abs1, abs2);
			int ggt = min(abs1, abs2);
			int nextop = next % ggt;
			while (nextop != 0) {
				next = ggt;
				ggt = nextop;
				nextop = next % ggt;
			}
			stepX /= ggt;
			stepY /= ggt;
		}
		return Point(stepX, stepY);
	}

	static bool checkLos(map<string, string> &field, unsigned x1, unsigned y1, unsigned x2, unsigned y2) {
			Point step = getStepSize(x1, y1, x2, y2);
			return checkDir(field, x1, y1, x2, y2, step);
	}

	static map<string, Point> mapSlopeAndLosForAst(map<string, string> &field, map<string, int> &losCount, unsigned width, unsigned height, unsigned x, unsigned y, string printTarget) {
		string targetKey = createKey(x, y);
		map<string, Point> slopeMap;
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
						slopeMap[key] = getStepSize(x, y, x1, y1);
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
		return slopeMap;
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
					mapSlopeAndLosForAst(field, losCount, width, height, x, y, printTarget);
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
		mapLos(field, losCount, width, height);
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

		// Pt1 result x,y
		int x = 19;
		int y = 11;
		map<string, Point> &slopeMap = mapSlopeAndLosForAst(field, losCount, width, height, x, y, "19,11");
		map<double, string> atanValues;
		auto it2 = slopeMap.begin();
		do {
			Point p = it2->second;
			double atan = atan2(p.y, p.x) * 180 / 3.141592653;
			atanValues[atan] = it2->first;
			it2++;
		} while (it2 != slopeMap.end());

		auto it3 = atanValues.begin();
		bool begin = false;
		int destroyedCount = 0;
		do {
			int angle = (int)it3->first;
			if (angle == -90)
				begin = true;
			if (begin)
				destroyedCount++;
			if (destroyedCount == 200)
				cout << "Result Pt2: " << it3->second << endl;
			it3++;
			if (it3 == atanValues.end())
				it3 = atanValues.begin();
		} while (destroyedCount < 200);
	}
};