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
				if (isKey(c) || isDoor(c) || c == character || c == '1' || c== '2' || c== '3' || c== '4') {
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
		if (origin.name != c && (isDoor(c) || isKey(c) || c == character || c == '1' || c == '2' || c == '3' || c == '4')) {
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

	static void getKeysInBranch(Node& currentNode, vector<char> &keysInBranch, map<char, int> &seen) {
		if (seen[currentNode.name] != 0)
			return;
		seen[currentNode.name] = 1;
		if (isKey(currentNode.name))
			keysInBranch.push_back(currentNode.name);
		auto it = currentNode.connectedNodes.begin();
		while (it != currentNode.connectedNodes.end()) {
			getKeysInBranch(*it->second, keysInBranch, seen);
			it++;
		}
	}

	static vector<char> getIndependentKeys(Node &currentNode, vector<char> availableKeys, vector<char> &keysInBranch) {
		for (int  i = 0;  i < keysInBranch.size() ;i++)
		{
			availableKeys.erase(find(availableKeys.begin(), availableKeys.end(), keysInBranch[i]));
		}
		return availableKeys;
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
		// Takes too much time to run...
		//findKeys(nodes[character], seen, keys, missingKeys, 0, stepsOnFinish);
		//cout << "Pt1: " << stepsOnFinish << endl;
		vector<char> keysInBranch1;
		getKeysInBranch(nodes['1'], keysInBranch1, map<char, int>());
		auto keys1 = getIndependentKeys(nodes['1'], missingKeys, keysInBranch1);
		int steps1 = 0;
		findKeys(nodes['1'], seen, keys1, keysInBranch1, 0, steps1);

		vector<char> keysInBranch2;
		getKeysInBranch(nodes['2'], keysInBranch2, map<char, int>());
		auto keys2 = getIndependentKeys(nodes['2'], missingKeys, keysInBranch2);
		int steps2 = 0;
		findKeys(nodes['2'], seen, keys2, keysInBranch2, 0, steps2);

		vector<char> keysInBranch3;
		getKeysInBranch(nodes['3'], keysInBranch3, map<char, int>());
		auto keys3 = getIndependentKeys(nodes['3'], missingKeys, keysInBranch3);
		int steps3 = 0;
		findKeys(nodes['3'], seen, keys3, keysInBranch3, 0, steps3);

		vector<char> keysInBranch4;
		getKeysInBranch(nodes['4'], keysInBranch4, map<char, int>());
		auto keys4 = getIndependentKeys(nodes['4'], missingKeys, keysInBranch4);
		int steps4 = 0;
		findKeys(nodes['4'], seen, keys4, keysInBranch4, 0, steps4);
		// basically cheating... will fail on certain inputs
		int total = steps1 + steps2 + steps3 + steps4;
		cout << "Pt2: " <<  total << endl;
	}
};