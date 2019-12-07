#include "stdafx.h"
#include "Utilities.cpp"
#include <map>

class Day6 {
	class Orbiter {
	public:
		string name;
		int nestingLevel;
		Orbiter *parent;
		vector<Orbiter*> children;

		Orbiter() {
			this->name = "";
			this->nestingLevel = 0;
			this->parent = nullptr;
		}

		Orbiter(string name) {
			this->name = name;
			this->nestingLevel = 0;
			this->parent = nullptr;
		}
	};

	static void initOrbiter(map<string, Orbiter> &orbits, string name) {
		auto search = orbits.find(name);
		if (search == orbits.end()) {
			orbits[name] = Orbiter(name);
		}
	}

	static void countOrbits(Orbiter &node, unsigned &direct, unsigned &indirect) {
		if (node.parent != nullptr) {
			direct++;
			indirect += node.parent->nestingLevel;
			node.nestingLevel = node.parent->nestingLevel + 1;
		}
		for each(Orbiter *child in node.children) {
			countOrbits(*child, direct, indirect);
		}
	}

public:
	static void run() {
		vector<string> fileContent = Utilities::readFile("input/Day6.txt");
		vector<vector<string>> input;
		for each(string line in fileContent) {
			input.push_back(Utilities::splitString(line, ")"));
		}

		map<string, Orbiter> orbits;

		for each(vector<string> orbit in input) {
			if (orbit.size() == 1) // EOF
				continue;
			string o1 = orbit[0];
			string o2 = orbit[1];
			initOrbiter(orbits, o1);
			initOrbiter(orbits, o2);
			orbits[o1].children.push_back(&orbits[o2]);
			orbits[o2].parent = &orbits[o1];
		}
		
		unsigned direct = 0;
		unsigned indirect = 0;
		countOrbits(orbits["COM"], direct, indirect);

		cout << "Direct: " << direct << " Indirect: " << indirect << " Total: " << direct + indirect << endl;

	}
};