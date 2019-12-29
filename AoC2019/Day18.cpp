#include "stdafx.h"
#include "Utilities.cpp"
#include <algorithm>
#include <map>

class Day18 {
	enum tiles {wall='#', floor='.', character='@'};

	class Node {
	public:
		Point pos;
		char name;
		map<char, Node*> connectedNodes;
		map<char, vector<int>> distances;

		Node() {
			this->name = '.';
		}
		
		Node(int x, int y, char name) {
			this->pos = Point(x, y);
			this->name = name;
		}
	};

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


	static bool isInRange(char min, char max, char c) {
		return min <= c && c <= max;
	}

	static bool isKey(char c) {
			return isInRange('a', 'z', c);
	}

	static bool isDoor(char c) {
			return isInRange('A', 'Z', c);
	}

	static map<char, Node> getNodes(Grid<char> g, vector<char> &keys) {
		map<char, Node> nodes;
		for (long long y = g.yMin; y <= g.yMax; y++) {
			for (long long x = g.xMin; x <= g.xMax; x++) {
				char c = g.get(x, y);
				if (isKey(c) || isDoor(c) || c == character) {
					Node k(x, y, c);
					nodes[c] = k;
					if (isKey(c))
						keys.push_back(c);
				}
			}
		}
		return nodes;
	}

	static void bfs(Grid<char> &g, Grid<int> seen, Node &origin, Point pos, map<char, Node> &nodes, int steps) {
		if (pos.x < g.xMin || pos.x > g.xMax || pos.y < g.yMin || pos.y > g.yMax)
			return;
		char c = g.get(pos.x, pos.y);
		if (c == wall)
			return;
		if (origin.name != c && (isDoor(c) || isKey(c) || c == character)) {
			Node* target = &nodes[c];
			origin.connectedNodes[c] = target;
			origin.distances[c].push_back(steps);
			return;
		}
		if (seen.seenPos(pos.x, pos.y) && steps > seen.get(pos.x, pos.y))
			return;

		seen.put(pos.x, pos.y, steps);
		steps++;

		bfs(g, seen, origin, Point(pos.x - 1, pos.y), nodes, steps);
		bfs(g, seen, origin, Point(pos.x + 1, pos.y), nodes, steps);
		bfs(g, seen, origin, Point(pos.x, pos.y - 1), nodes, steps);
		bfs(g, seen, origin, Point(pos.x, pos.y + 1), nodes, steps);
	}

	static void getEdges(Grid<char> &g, map<char, Node> &nodes) {
		auto it = nodes.begin();
		while (it != nodes.end()) {
			Grid<int> seen;
			bfs(g, seen, it->second, it->second.pos, nodes, 0);
			it++;
		}
	}

	class State {
		vector<char> keys;
		int steps;
	};
	
	static string makeStateKey(vector<char> &keys) {
		string key = "";
		for (auto i = keys.begin(); i != keys.end(); i++)
		{
			key = key + *i;
		}

		return key;
	}

	static void findKeys(Node& currentNode, map<char, map<string, int>> &seen, vector<char> keys, vector<char> missingKeys, int totalSteps, int &stepsOnFinish) {
		if (stepsOnFinish != 0 && totalSteps >= stepsOnFinish)
			return;
		if (isKey(currentNode.name) && find(keys.begin(), keys.end(), currentNode.name) == keys.end()) {
			keys.push_back(currentNode.name);
			sort(keys.begin(), keys.end());
			missingKeys.erase(find(missingKeys.begin(), missingKeys.end(), currentNode.name));
		}
		string key = makeStateKey(keys);
		if (seen[currentNode.name][key] != 0 && totalSteps >= seen[currentNode.name][key])
			return;
		/*auto si = seen[currentNode.name].begin();
		while (si != seen[currentNode.name].end()) {
			string str = si->first;
			if (str.size() < keys.size()) {
				si++;
				continue;
			}
				
			bool foundall = true;
			for (int i = 0; i < keys.size(); i++)
			{
				if (find(str.begin(), str.end(), keys[i]) == str.end()) {
					foundall = false;
					break;
				}
			}
			if (foundall && si->second != 0 && totalSteps >= si->second)
				return;
			si++;
		}*/
		seen[currentNode.name][key] = totalSteps;


		if (missingKeys.size() == 0) {
			if (stepsOnFinish != 0 && stepsOnFinish < totalSteps)
				throw;
			stepsOnFinish = totalSteps;
			return;
		}

		auto it = currentNode.connectedNodes.begin();
		while (it != currentNode.connectedNodes.end()) {
			for (int idx = 0; idx < currentNode.distances[it->first].size(); idx++)
			{
				Node& n = *it->second;
				if (isDoor(n.name) && find(keys.begin(), keys.end(), tolower(n.name)) == keys.end())
					continue;

				findKeys(*it->second, seen, keys, missingKeys, totalSteps + currentNode.distances[it->first][idx], stepsOnFinish);
			}
			it++;
		}
	}

public:
	static void run() {
		auto input = Utilities::readFile("input/Day18.txt");
		Grid<char> g = createGrid(input);
		vector<char> missingKeys;
		map<char, Node> nodes = getNodes(g, missingKeys);
		getEdges(g, nodes);
		vector<char> keys;
		int stepsOnFinish = 0;
		map<char, map<string, int>> seen;
		findKeys(nodes[character], seen, keys, missingKeys, 0, stepsOnFinish);
		cout << "Pt1: " << stepsOnFinish << endl;
	}
};