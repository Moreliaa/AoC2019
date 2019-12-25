#include "stdafx.h"
#include "Utilities.cpp"
#include <algorithm>

class Day18 {
	enum tiles {wall='#', floor='.', character='@'};

	static Grid<char> createGrid(vector<string> &input) {
		Grid<char> g;
		long long x = 0;
		long long y = 0;
		for each (string line in input)
		{
			if (line == "")
				break;
			for (unsigned i = 0; i < line.size(); i++)
			{
				char chara = line[i];
				g.put(x, y, chara);
				x++;
			}
			x = 0;
			y++;
		}
		return g;
	}

	static Point getStartPos(Grid<char> g) {
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				if (g.get(x, y) == character)
					return Point(x, y);
			}
		}
		throw;
	}

	static bool isInRange(char min, char max, char c) {
		int ascii_min = stoi(to_string(min));
		int ascii_max = stoi(to_string(max));
		int ascii_c = stoi(to_string(c));
		return ascii_min <= ascii_c && ascii_c <= ascii_max;
	}

	static bool isKey(char c) {
		return isInRange('a', 'z', c);
	}

	static bool isDoor(char c) {
		return isInRange('A', 'Z', c);
	}

	static vector<char> getKeyList(Grid<char> g) {
		vector<char> keys;
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				char c = g.get(x, y);
				if (isKey(c))
					keys.push_back(c);
			}
		}
		return keys;
	}

	static void bfs(Grid<char> g, Grid<int> seen, Point pos, vector<char> keys, vector<char> missingKeys, vector<int> &stepsOnFinish, int steps) {
		if (pos.x < g.xMin || pos.x > g.xMax || pos.y < g.yMin || pos.y > g.yMax)
			return;
		char c = g.get(pos.x, pos.y);
		if (c == wall)
			return;
		if (isDoor(c) && find(keys.begin(), keys.end(), tolower(c)) == keys.end())
			return;
		if (seen.seenPos(pos.x, pos.y) && steps > seen.get(pos.x, pos.y))
			return;

		if (isKey(c)) {
			keys.push_back(c);
			missingKeys.erase(find(missingKeys.begin(), missingKeys.end(), c));
			g.put(pos.x, pos.y, floor);
			if (missingKeys.size() == 0) {
				stepsOnFinish.push_back(steps);
				return;
			}
			seen.reset();
		}

		seen.put(pos.x, pos.y, steps);
		steps++;

		bfs(g, seen, Point(pos.x - 1, pos.y), keys, missingKeys, stepsOnFinish, steps);
		bfs(g, seen, Point(pos.x + 1, pos.y), keys, missingKeys, stepsOnFinish, steps);
		bfs(g, seen, Point(pos.x, pos.y - 1), keys, missingKeys, stepsOnFinish, steps);
		bfs(g, seen, Point(pos.x, pos.y + 1), keys, missingKeys, stepsOnFinish, steps);
	}

public:
	static void run() {
		auto input = Utilities::readFile("input/Day18.txt");
		Grid<char> g = createGrid(input);
		Point start = getStartPos(g);
		vector<char> missingKeys = getKeyList(g);
		vector<int> stepsOnFinish;
		Grid<int> seen;
		vector<char> keys;
		bfs(g, seen, start, keys, missingKeys, stepsOnFinish, 0);
		auto minimum = min_element(stepsOnFinish.begin(), stepsOnFinish.end());
		cout << "Pt1: " << *minimum << endl;
	}
};