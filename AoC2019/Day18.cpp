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
		map<char, vector<vector<char>>> reqs;

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

	static void bfs(Grid<char> &g, Grid<int> seen, Node &origin, Point pos, map<char, Node> &nodes, int steps, vector<char> reqs) {
		if (pos.x < g.xMin || pos.x > g.xMax || pos.y < g.yMin || pos.y > g.yMax)
			return;
		char c = g.get(pos.x, pos.y);
		if (c == wall)
			return;
		if (isDoor(c))
			reqs.push_back(c);
		else if (origin.name != c && (isKey(c) || c == character)) {
			Node* target = &nodes[c];
			origin.connectedNodes[c] = target;
			origin.distances[c].push_back(steps);
			origin.reqs[c].push_back(reqs);
		}
		if (seen.seenPos(pos.x, pos.y) && steps > seen.get(pos.x, pos.y))
			return;

		seen.put(pos.x, pos.y, steps);
		steps++;

		bfs(g, seen, origin, Point(pos.x - 1, pos.y), nodes, steps, reqs);
		bfs(g, seen, origin, Point(pos.x + 1, pos.y), nodes, steps, reqs);
		bfs(g, seen, origin, Point(pos.x, pos.y - 1), nodes, steps, reqs);
		bfs(g, seen, origin, Point(pos.x, pos.y + 1), nodes, steps, reqs);
	}

	static void getEdges(Grid<char> &g, map<char, Node> &nodes) {
		auto it = nodes.begin();
		while (it != nodes.end()) {
			Grid<int> seen;
			vector<char> reqs;
			bfs(g, seen, it->second, it->second.pos, nodes, 0, reqs);
			it++;
		}
	}

	static void findKeys(Node& currentNode, map<char, int> seen, vector<char> keys, vector<char> missingKeys, int totalSteps, vector<int> &stepsOnFinish) {
		if (seen[currentNode.name] != 0)
			return;
		seen[currentNode.name] = 1;
		if (isKey(currentNode.name) && find(keys.begin(), keys.end(), currentNode.name) == keys.end()) {
			keys.push_back(currentNode.name);
			missingKeys.erase(find(missingKeys.begin(), missingKeys.end(), currentNode.name));
		}

		if (missingKeys.size() == 0) {
			stepsOnFinish.push_back(totalSteps);
			return;
		}

		auto it = currentNode.connectedNodes.begin();
		while (it != currentNode.connectedNodes.end()) {
			for (int idx = 0; idx < currentNode.distances[it->first].size(); idx++)
			{
				bool reqsFulfilled = true;
				Node n = *it->second;
				for (auto itreqs = currentNode.reqs[n.name][idx].begin(); itreqs != currentNode.reqs[n.name][idx].end(); itreqs++)
				{
					if (find(keys.begin(), keys.end(), tolower(*itreqs)) == keys.end())
						reqsFulfilled = false;
				}
				if (!reqsFulfilled)
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
		vector<int> stepsOnFinish;
		map<char, int> seen;
		findKeys(nodes[character], seen, keys, missingKeys, 0, stepsOnFinish);
		auto minimum = min_element(stepsOnFinish.begin(), stepsOnFinish.end());
		cout << "Pt1: " << *minimum << endl;
	}
};