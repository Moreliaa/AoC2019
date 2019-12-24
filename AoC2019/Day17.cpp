#include "stdafx.h"
#include "IntcodeC.cpp"
#include <sstream>
#include "Utilities.cpp"

class Day17 {
	enum directions { U, R, D, L };
	enum directionChange { counterclockwise, clockwise };

	static Point move(int x, int y, directions orientation) {
		Point p;
		switch (orientation) {
		case U:
			y--;
			p = Point(x, y);
			return p;
		case R:
			x++;
			p = Point(x, y);
			return p;
		case D:
			y++;
			p = Point(x, y);
			return p;
		case L:
			x--;
			p = Point(x, y);
			return p;
		default:
			throw;
		}
	}

	static directions changeOrientation(directions currentOrientation, directionChange change) {
		int idx = currentOrientation + (change == 0 ? -1 : 1);
		directions idx_d = (directions)idx;
		if (idx_d > L)
			return U;
		else if (idx_d < U)
			return L;
		else
			return idx_d;
	}

	static Point getStartPos(Grid<char> g) {
		const char bot = '^';
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				if (g.get(x, y) == bot)
					return Point(x, y);
			}
		}
		throw;
	}

	static vector<string> mapCommands(Grid<char> g) {
		vector<string> commands;
		directions dir = U;
		Point pos = getStartPos(g);
		const char scaffold = '#';
		int counter;
		while (true) {
			counter = 0;
			do {
				Point next = move(pos.x, pos.y, dir);
				if (g.get(next.x, next.y) != scaffold)
					break;
				counter++;
				pos = next;
			} while (true);
			if (counter > 0)
				commands.push_back(to_string(counter));

			directions newDir = changeOrientation(dir, counterclockwise);
			Point next = move(pos.x, pos.y, newDir);
			if (g.get(next.x, next.y) == scaffold) {
				dir = newDir;
				commands.push_back("L");
			}
			else {
				newDir = changeOrientation(dir, clockwise);
				next = move(pos.x, pos.y, newDir);
				if (g.get(next.x, next.y) == scaffold) {
					dir = newDir;
					commands.push_back("R");
				}
				else {
					break;
				}
			}
		}
		return commands;
	}

	static void drawGrid(Grid<char> g) {
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				cout << g.get(x, y);
			}
			cout << endl;
		}
	}

	static int sumAlignmentParams(Grid<char> g) {
		int sum = 0;
		const char scaffold = '#';
		for (long long y = g.yMin + 1; y < g.yMax; y++) {
			for (long long x = g.xMin + 1; x < g.xMax; x++) {
				if (g.get(x, y) == scaffold &&
					g.get(x - 1, y) == scaffold &&
					g.get(x + 1, y) == scaffold &&
					g.get(x, y - 1) == scaffold &&
					g.get(x, y + 1) == scaffold) {
					sum += (x*y);
				}
			}
		}
		return sum;
	}

public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day17.txt");
		IntcodeProgram prog(input_cache);
		stringstream s_io;
		IntcodeC::runProgram(prog, s_io, s_io, false, true);
		string line;
		int x = 0;
		int y = 0;
		Grid<char> g;

		do {
			getline(s_io, line);
			if (line == "")
				break;
			char next = stoi(line);
			if (line == "10") {
				x = 0;
				y++;
			}
			else {
				g.put(x, y, next);
				x++;
			}
		} while (true);

		drawGrid(g);
		cout << "Pt1: " << sumAlignmentParams(g) << endl;

		vector<string> commandList = mapCommands(g);

		/*
		A,B,A,B,A,C,B,C,A,C

		A: L,5,5,L,6,6,R,6
		B: R,5,5,L,4,L,4,L,6,6
		C: L,5,5,R,5,5,R,6,L,4
		*/

		input_cache[0] = 2;
		string commandString = "A,B,A,B,A,C,B,C,A,C\nL,5,5,L,6,6,R,6\nR,5,5,L,4,L,4,L,6,6\nL,5,5,R,5,5,R,6,L,4\nn\n";
		stringstream s_io2;
		IntcodeProgram prog2(input_cache);
		for (unsigned i = 0; i < commandString.size(); i++)
		{
			int chara = commandString[i];
			IntcodeC::runProgram(prog2, s_io2, cout, false, true);
			s_io2 << chara << endl;
			prog2.nextInputAvailable = true;
		}

		IntcodeC::runProgram(prog2, s_io2, cout, false, false);

	}
};