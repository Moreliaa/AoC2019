#include "stdafx.h"
#include "IntcodeC.cpp"
#include <sstream>
#include "Utilities.cpp"

class Day19 {
	static int drawGrid(Grid<int>& g) {
		int affectedTiles = 0;
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				int a = g.get(x, y);
				if (a == 0)
					cout << ".";
				else if (a == 1) {
					cout << "#";
					affectedTiles++;
				}
				else
					throw;
			}
			cout << endl;
		}
		return affectedTiles;
	}

	static int getOutput(map<long long, long long>& input, long long x, long long y) {
		if (x < 0 || y < 0)
			return 0;
		stringstream s_in;
		stringstream s_out;
		s_in << x << endl;
		s_in << y << endl;
		IntcodeProgram prog(input);
		IntcodeC::runProgram(prog, s_in, s_out, true, false);

		string line;
		getline(s_out, line);
		return stoi(line);
	}


public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day19.txt");
		Grid<int> g;
		long long stepSizeY = 1;
		long long xmin = 0;
		for (long long y = 100; true; y++) {
			bool seen = false;
			for (long long x = xmin; true; x++) {
				int out = getOutput(input_cache, x, y);
				//g.put(x, y, out);

				if (out == 1) {
					seen = true;
					xmin = x;
				}
				else if (seen) {
					int out100x = getOutput(input_cache, x - 100, y);
					if (out100x != 1)
						break;
					int out100y = getOutput(input_cache, x - 100, y + 99);
					if (out100y != 1)
						break;
					cout << "Pt2: " << (x - 100) * 10000 + y << endl;
						return;
				}
			}
		}
		int tiles = drawGrid(g);
		cout << "Tiles Pt1: " << tiles << endl;
	}
};