#include "stdafx.h"
#include "Utilities.cpp"
#include <algorithm>
#include <map>

class Day20 {

	class Portal {
	public:
		string name = "";
		Point p1;
		Point p2;

		Portal() {

		}

		Portal(string name) {
			this->name = name;
		}
	};

	static Grid<char> createGrid(vector<string>& input) {
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


	static bool isInRange(char min, char max, char c) {
		return min <= c && c <= max;
	}

	static bool isDoor(char c) {
		return isInRange('A', 'Z', c);
	}

	static Point lookForFloorTile(Grid<char> &g, Point p1, Point p2) {
		long long x = p1.x;
		long long y = p1.y;
		char c1 = g.get(x + 1, y);
		if (c1 == '.') {
			Point result(x+1, y);
			return result;
		}
		char c2 = g.get(x, y + 1);
		if (c2 == '.') {
			Point result(x, y + 1);
			return result;
		}
		char c3 = g.get(x - 1, y);
		if (c3 == '.') {
			Point result(x - 1, y);
			return result;
		}
		char c4 = g.get(x, y - 1);
		if (c4 == '.') {
			Point result(x, y - 1);
			return result;
		}
		x = p2.x;
		y = p2.y;
		c1 = g.get(x + 1, y);
		if (c1 == '.') {
			Point result(x + 1, y);
			return result;
		}
		c2 = g.get(x, y + 1);
		if (c2 == '.') {
			Point result(x, y + 1);
			return result;
		}
		c3 = g.get(x - 1, y);
		if (c3 == '.') {
			Point result(x - 1, y);
			return result;
		}
		c4 = g.get(x, y - 1);
		if (c4 == '.') {
			Point result(x, y - 1);
			return result;
		}
		throw;
	}

	static map<string, Portal> getNodes(Grid<char> &g) {
		map<string, Portal> nodes;
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				char c = g.get(x, y);
				if (isDoor(c)) {
					char c1 = g.get(x + 1, y);
					char c2 = g.get(x, y + 1);
					if (isDoor(c1)) {
						Point floor = lookForFloorTile(g, Point(x, y), Point(x + 1, y));
						string key = string(1, c) + string(1, c1);
						auto search = nodes.find(key);
						if (search == nodes.end()) {
							nodes[key] = Portal(key);
							nodes[key].p1 = floor;
						}
						else {
							nodes[key].p2 = floor;
						}
					} else if (isDoor(c2)) {
						Point floor = lookForFloorTile(g, Point(x, y), Point(x, y + 1));
						string key = string(1, c) + string(1, c2);
						auto search = nodes.find(key);
						if (search == nodes.end()) {
							nodes[key] = Portal(key);
							nodes[key].p1 = floor;
						}
						else {
							nodes[key].p2 = floor;
						}
					}
				}
			}
		}
		return nodes;
	}

	static Portal getPortal(Grid<char> &g, map<string, Portal> &portals, Point& pos, char c) {
		long long x = pos.x;
		long long y = pos.y;
		char c1 = g.get(x + 1, y);
		char c2 = g.get(x, y + 1);
		char c3 = g.get(x - 1, y);
		char c4 = g.get(x, y - 1);
		bool d1 = isDoor(c1);
		bool d2 = isDoor(c2);
		bool d3 = isDoor(c3);
		bool d4 = isDoor(c4);
		string key;
		if (d1)
			key = string(1, c) + string(1, c1);
		else if (d2)
			key = string(1, c) + string(1, c2);
		else if (d3)
			key = string(1, c3) + string(1, c);
		else if (d4)
			key = string(1, c4) + string(1, c);
		return portals[key];
	}

	static void bfs(Grid<char>& g, Grid<int> &seen, map<string, Portal> &portals, Point pos, Point lastPos, int steps) {
		if (pos.x < g.xMin || pos.x > g.xMax || pos.y < g.yMin || pos.y > g.yMax)
			return;
		char c = g.get(pos.x, pos.y);
		if (c == '#')
			return;
		if (isDoor(c)) {
			Portal portal = getPortal(g, portals, pos, c);
			if (portal.name == "AA")
				return;
			if (portal.name == "ZZ") {
				return;
			}
			if (lastPos.x == portal.p1.x && lastPos.y == portal.p1.y) {
				pos.x = portal.p2.x;
				pos.y = portal.p2.y;
			}
			else {
				pos.x = portal.p1.x;
				pos.y = portal.p1.y;
			}
		}
		if (seen.seenPos(pos.x, pos.y) && steps >= seen.get(pos.x, pos.y))
			return;
		seen.put(pos.x, pos.y, steps);
		steps++;

		bfs(g, seen, portals, Point(pos.x - 1, pos.y), pos, steps);
		bfs(g, seen, portals, Point(pos.x + 1, pos.y), pos, steps);
		bfs(g, seen, portals, Point(pos.x, pos.y - 1), pos, steps);
		bfs(g, seen, portals, Point(pos.x, pos.y + 1), pos, steps);
	}


public:
	static void run() {
		auto input = Utilities::readFile("input/Day20.txt");
		Grid<char> g = createGrid(input);
		map<string, Portal> portals = getNodes(g);
		Grid<int> seen;
		bfs(g, seen, portals, portals["AA"].p1, portals["AA"].p1, 0);
		
		cout << "Pt1: " << seen.get(portals["ZZ"].p1.x, portals["ZZ"].p1.y) << endl;
	}
};