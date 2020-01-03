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
				if (chara == 32)
					chara = '#';
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

	static bool isOuterRing(Grid<char>& g, Point& pos) {
		return pos.x == 2 || pos.y == 2 || pos.x == g.xMax - 2 || pos.y == g.yMax - 2;
	}

	static void dijkstra(Grid<char>& g, Grid<int> &distances, Grid<int>& visited, Grid<int>& unvisited, map<string, Portal> &portals, Point pos, int recursionLevel, int steps) {
		distances.put(pos.x, pos.y, steps, to_string(recursionLevel));
		visited.put(pos.x, pos.y, 1, to_string(recursionLevel));
		auto unvis = unvisited.grid.find(Grid<int>::createKey(pos.x, pos.y, to_string(recursionLevel)));
		if (unvis != unvisited.grid.end())
			unvisited.grid.erase(unvis);
		steps++;

		vector<Point> points = { 
			Point(pos.x - 1, pos.y),
			Point(pos.x + 1, pos.y),
			Point(pos.x, pos.y - 1),
			Point(pos.x, pos.y + 1)
		};
		for each (Point p in points)
		{
			int nextRecursionLevel = recursionLevel;
			if (p.x < g.xMin || p.x > g.xMax || p.y < g.yMin || p.y > g.yMax) {
				continue;
			}
			char c = g.get(p.x, p.y);
			if (c == '#') {
				continue;
			}
			if (isDoor(c)) {
				bool isOuter = isOuterRing(g, pos);
				Portal portal = getPortal(g, portals, p, c);
				if (nextRecursionLevel == 0 && isOuter) {
					if (portal.name == "ZZ") {
						return; // done
					}
					else { // wall
						continue;
					}
				} else if (nextRecursionLevel != 0 && (portal.name == "AA" || portal.name == "ZZ"))
				{ // wall
					continue;
				}
				
				if (pos.x == portal.p1.x && pos.y == portal.p1.y) {
					p.x = portal.p2.x;
					p.y = portal.p2.y;
				}
				else {
					p.x = portal.p1.x;
					p.y = portal.p1.y;
				}
				if (isOuter)
					nextRecursionLevel--;
				else
					nextRecursionLevel++;
			}

			bool hasEntry = distances.seenPos(p.x, p.y, to_string(nextRecursionLevel));
			if (hasEntry)
				continue;
			distances.put(p.x, p.y, steps, to_string(nextRecursionLevel));
			unvisited.put(p.x, p.y, 1, to_string(nextRecursionLevel));
		}
		
	}


public:
	static void run() {
		auto input = Utilities::readFile("input/Day20.txt");
		Grid<char> g = createGrid(input);
		map<string, Portal> portals = getNodes(g);
		Grid<int> distances;
		Grid<int> visited;
		Grid<int> unvisited;
		dijkstra(g, distances, visited, unvisited, portals, portals["AA"].p1, 0, 0);
		while (!distances.seenPos(portals["ZZ"].p1.x, portals["ZZ"].p1.y, "0")) {
			vector<string> splitKey;
			int newx = -1;
			int newy = -1;
			int newRec = -1;
			int nextSteps = -1;
			string nextKey;
			for (auto it = unvisited.grid.begin(); it != unvisited.grid.end(); it++)
			{
					int s = distances.grid[it->first];
					if (s < nextSteps || nextSteps == -1)
					{
						nextSteps = s;
						nextKey = it->first;
						splitKey = Utilities::splitString(nextKey, ",");
						newRec = stoi(splitKey[2]);
					}
					else if (s == nextSteps) {
						vector<string> checkedKey = Utilities::splitString(it->first, ",");
						int  checkedRec = stoi(checkedKey[2]);
						if (checkedRec < newRec) {
							splitKey = checkedKey;
							newRec = checkedRec;
						}
					}
			}
			newx = stoi(splitKey[0]);
			newy = stoi(splitKey[1]);
			
			
			dijkstra(g, distances, visited, unvisited, portals, Point(newx, newy), newRec, nextSteps);
		}
		
		cout << "Pt2: " << distances.get(portals["ZZ"].p1.x, portals["ZZ"].p1.y, "0") << endl;
	}
};