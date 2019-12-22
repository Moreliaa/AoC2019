#include "stdafx.h"
#include "IntcodeC.cpp"
#include <sstream>
#include "Utilities.cpp"

class Day15 {
	enum moves {north=1,south=2,west=3,east=4};
	enum scode {wall, succ, oxygen};

	class Droid {

		Point move(moves m, int x, int y) {
			Point p;
			switch (m) {
			case north:
				p = Point(x, y - 1);
				return p;
			case east:
				p = Point(x + 1, y);
				return p;
			case south:
				p = Point(x, y + 1);
				return p;
			case west:
				p = Point(x - 1, y);
				return p;
			default:
				throw;
			}
		}

		void tryMovement(IntcodeProgram progCopy, moves m, long long x, long long y, int steps) {
			steps++;
			Point next = move(m, (int)x, (int)y);
			long long x_next = next.x;
			long long y_next = next.y;
			if (g.seenPos(x_next, y_next) && steps > g.get(x_next, y_next))
				return;

			long long returnCode;
			stringstream s_io;
			s_io << m << endl;
			returnCode = IntcodeC::runProgram(progCopy, s_io, s_io, true);
			string status;
			getline(s_io, status);
			moves status_i = (moves) stoi(status);
			g.put(x_next, y_next, steps);
			statusMap.put(x_next, y_next, status_i);
			switch (status_i) {
			case wall:
				return;
			case succ:
				mapEnvironment(progCopy, x_next, y_next, steps);
				return;
			case oxygen:
				if (minSteps == -1 || steps < minSteps) {
					minSteps = steps;
					oxygenLoc = Point(x, y);
					progStateAtOxygen = progCopy;
				}
				return;
			default:
				throw;
			}
		}


	public:
		Grid<int> g;
		Grid<int> statusMap;
		IntcodeProgram prog;
		int minSteps = -1;
		Point oxygenLoc;
		IntcodeProgram progStateAtOxygen;

		Droid(IntcodeProgram prog) {
			this->prog = prog;
			g.put(0, 0, 0);
			statusMap.put(0, 0, 99);
		}

		void mapEnvironment(IntcodeProgram progCopy, long long x, long long y, int steps) {
			tryMovement(progCopy, west, x, y, steps);
			tryMovement(progCopy, north, x, y, steps);
			tryMovement(progCopy, south, x, y, steps);
			tryMovement(progCopy, east, x, y, steps);
		}

		void mapOxygen() {
			g.reset();
			mapEnvironment(progStateAtOxygen, oxygenLoc.x, oxygenLoc.y, 0);
			int maxSteps = 0;
			for (auto it = g.grid.begin(); it != g.grid.end(); it++)
			{
				maxSteps = max(maxSteps, it->second);
			}
			cout << "Time needed in mins: " << maxSteps << endl;
		}

		
	};

	 static void drawGrid(Grid<int> &g) {
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				scode a = (scode) g.get(x, y);
				if (a == wall)
					cout << "#";
				else if (a == succ)
					cout << ".";
				else if (a == 99)
					cout << "@"; // start
				else if (a == oxygen)
					cout << "?";
				else
					cout << "+";
			}
			cout << endl;
		}
	}
	

public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day15.txt");
		IntcodeProgram prog(input_cache);
		Droid d(prog);
		d.mapEnvironment(prog, 0, 0, 0);
		cout << "Steps: " << d.minSteps << endl;
		drawGrid(d.statusMap);
		d.mapOxygen();
	}
};