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
			int status_i = stoi(status);
			g.put(x_next, y_next, steps);
			switch (status_i) {
			case wall:
				return;
			case succ:
				mapEnvironment(progCopy, x_next, y_next, steps);
				return;
			case oxygen:
				if (minSteps == -1 || steps < minSteps) {
					minSteps = steps;
				}
				return;
			default:
				throw;
			}
		}


	public:
		Grid<int> g;
		IntcodeProgram prog;
		int minSteps = -1;

		Droid(IntcodeProgram prog) {
			this->prog = prog;
			g.put(0, 0, 0);
		}

		void mapEnvironment(IntcodeProgram progCopy, long long x, long long y, int steps) {
			tryMovement(progCopy, north, x, y, steps);
			tryMovement(progCopy, west, x, y, steps);
			tryMovement(progCopy, south, x, y, steps);
			tryMovement(progCopy, east, x, y, steps);
		}

		
	};
	

public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day15.txt");
		IntcodeProgram prog(input_cache);
		Droid d(prog);
		d.mapEnvironment(prog, 0, 0, 0);
		cout << "Steps: " << d.minSteps << endl;
	}
};