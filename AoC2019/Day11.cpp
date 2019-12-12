#include "stdafx.h"
#include "IntcodeC.cpp"
#include <map>
#include <sstream>
#include <algorithm>

class Day11 {
	enum directions {U, R, D, L};
	enum directionChange {counterclockwise, clockwise};
	enum palette {black, white};

	class PanelMap {
		int xMax = 0;
		int xMin = 0;
		int yMax = 0;
		int yMin = 0;
		map<string, palette> colors;

		string getKey(int x, int y) {
			return to_string(x) + "," + to_string(y);
		}

	public:
		PanelMap() {

		}

		PanelMap(int x, int y, palette initialColor) {
			colors[getKey(x, y)] = initialColor;
		}

		void paint(int x, int y, palette color) {
			xMax = max(x, xMax);
			xMin = min(x, xMin);
			yMax = max(y, yMax);
			yMin = min(y, yMin);
			string key = this->getKey(x, y);
			this->colors[key] = color;
		}

		palette getColor(int x, int y) {
			string key = this->getKey(x, y);
			auto search = colors.find(key);
			if (search == colors.end()) {				
				return black;
			}
			return this->colors[key];
		}

		void printNumberOfPaintedPanels() {
			cout << "Pt1: " << this->colors.size() << endl;
		}

		void printPanels() {
			for (int y = yMin; y <= yMax; y++)
			{
				for (int x = xMin; x <= xMax; x++)
				{
					if (colors[getKey(x, y)] == white)
						cout << "#";
					else
						cout << " ";
				}
				cout << endl;
			}
		}
	};

	class Robot {
		IntcodeProgram prog;
		PanelMap* panels;
		int x;
		int y;
		directions orientation;

		void move() {
			switch (this->orientation) {
			case U:
				this->y--;
				break;
			case R:
				this->x++;
				break;
			case D:
				this->y++;
				break;
			case L:
				this->x--;
				break;
			default:
				throw;
			}
		}

		void changeOrientation(directionChange change) {
			int idx = this->orientation + (change == 0 ? -1 : 1);
			directions idx_d = (directions) idx;
			if (idx_d > L)
				this->orientation = U;
			else if (idx_d < U)
				this->orientation = L;
			else
				this->orientation = idx_d;
		}

		int getNextInput() {
			palette color = panels->getColor(x, y);
			if (color == black)
				return 0;
			if (color == white)
				return 1;
			throw;
		}

	public:
		Robot(IntcodeProgram prog, PanelMap &panels, int x, int y) {
			this->prog = prog;
			this->panels = &panels;
			this->x = x;
			this->y = y;
			this->orientation = U;
		}

		void run() {
			long long returnCode;
			do {
				stringstream s_io;
				s_io << getNextInput() << endl;

				returnCode = IntcodeC::runProgram(prog, s_io, s_io, true);
				if (returnCode == 99)
					break;
				string color;
				getline(s_io, color);
				panels->paint(x, y, (palette)stoi(color));

				returnCode = IntcodeC::runProgram(prog, s_io, s_io, true);
				if (returnCode == 99)
					break;
				string rotation;
				getline(s_io, rotation);
				changeOrientation((directionChange)stoi(rotation));
				move();
			} while (true);
		}
	};

public:
	static void run() {
		auto input = IntcodeC::getInput("input/Day11.txt");
		PanelMap panels;
		Robot rob(IntcodeProgram(input), panels, 0, 0);
		rob.run();
		panels.printNumberOfPaintedPanels();
		PanelMap panels2(0,0, white);
		Robot rob2(IntcodeProgram(input), panels2, 0, 0);
		rob2.run();
		panels2.printPanels();
	}
};