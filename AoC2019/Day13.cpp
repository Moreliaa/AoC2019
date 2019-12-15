#include "stdafx.h"
#include "Utilities.cpp"
#include "IntcodeC.cpp"
#include <map>
#include <sstream>

class Day13 {
	enum tiles {empty, wall, block, paddle, ball};
	enum joystick {left=-1, neutral=0, right=1};

	static string getKey(int x, int y) {
		return to_string(x) + "," + to_string(y);
	}

	class Game {
	public:
		map<string, tiles> screen;
		IntcodeProgram gamelogic;
		stringstream s_in;
		stringstream s_out;
		int blocks;
		int score = -1;
		Point ballPos;
		Point paddlePos;

		Game(map<long long, long long> &input) {
			this->gamelogic = IntcodeProgram(input);
		}

		void sendNextInput() {
			int diff = paddlePos.x - ballPos.x;
			string nextInput;
			if (diff < 0)
				nextInput = to_string(right);
			else if (diff > 0)
				nextInput = to_string(left);
			else
				nextInput = to_string(neutral);

			s_in << nextInput << endl;
			gamelogic.nextInputAvailable = true;
		}

		void updateState(stringstream &out) {
			string x, y, tile;
			int x_i;
			int y_i;
			int tile_i;
			int newblocks = -1;
			do {
				getline(out, x);
				if (x == "") {					
					break;
				}
				getline(out, y);
				getline(out, tile);
				x_i = stoi(x);
				y_i = stoi(y);
				tile_i = stoi(tile);

				if ((tiles)tile_i == block) {
					newblocks++;
				} else if ((tiles)tile_i == ball) {
					ballPos.x = x_i;
					ballPos.y = y_i;
				} else if ((tiles)tile_i == paddle) {
					paddlePos.x = x_i;
					paddlePos.y = y_i;
				}

				if (x_i == -1 && y_i == 0)
					score = tile_i;
				else
					screen[getKey(x_i, y_i)] = (tiles)tile_i;
			} while (true);
			if (newblocks != -1)
				blocks = newblocks + 1;
		}

		long long run() {
			stringstream out;
			long long returnCode = IntcodeC::runProgram(gamelogic, s_in, out, false, true);
			updateState(out);
			sendNextInput();
			return returnCode;
		}
	};

public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day13.txt");
		auto input(input_cache);
		Game game(input);
		game.run();
		cout << "Pt1: " << game.blocks << endl;

		input_cache[0] = 2;
		Game game2(input_cache);
		long long rc;
		do {
			rc = game2.run();
		} while (rc != 99);

		cout << "Pt2 - Remaining blocks: " << game2.blocks << " Score: " << game2.score << endl;
	}
};