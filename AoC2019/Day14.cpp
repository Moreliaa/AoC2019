#include "stdafx.h"
#include "Utilities.cpp"
#include <regex>
#include <map>

class Day14 {
	class Reagent {
	public:
		int count;
		string name;
		Reagent() {

		}
		Reagent(int count, string name) {
			this->count = count;
			this->name = name;
		}
	};

	class Reaction {
	public:
		vector<Reagent> reagents;
		Reagent output;

		Reaction() {

		}

		Reaction(vector<Reagent> reagents, Reagent output) {
			this->reagents = reagents;
			this->output = output;
		}
	};

	static Reagent getReagent(string rg) {
		Utilities::trim(rg);
		vector<string> split = Utilities::splitString(rg, " ");
		return Reagent(stoi(split[0]), split[1]);
	}

	static vector<Reaction> getReactions(vector<string> &input) {
		vector<Reaction> reactions;
		for each (string line in input) {
			if (line == "")
				break;
			vector<string> split = Utilities::splitString(line, "=>");
			vector<string> reagentStr = Utilities::splitString(split[0], ",");
			vector<Reagent> reagents;
			for each (string str in reagentStr)
			{
				Reagent rg = getReagent(str);
				reagents.push_back(rg);
			}
			Reagent out = getReagent(split[1]);
			Reaction rea(reagents, out);
			reactions.push_back(rea);
		}
		return reactions;
	}

	static Reaction findReactionByOutput(vector<Reaction> reactions, string name) {
		for (unsigned i = 0; i < reactions.size(); i++)
		{
			if (reactions[i].output.name == name)
			{
				return reactions[i];
			}
		}
		throw;
	}

	static int addReagents(map<string, int> &excess, vector<Reaction> &reactions, Reaction &currentReaction, int numOutputRequired) {
		auto s = excess.find(currentReaction.output.name);
		if (s == excess.end())
			excess[currentReaction.output.name] = 0;

		while (numOutputRequired > 0 && excess[currentReaction.output.name] > 0) {
			numOutputRequired--;
			excess[currentReaction.output.name]--;
		}

		if (numOutputRequired == 0)
			return 0;

		int numberOfReactionsNeeded = 0;
		while (numberOfReactionsNeeded * currentReaction.output.count < numOutputRequired)
			numberOfReactionsNeeded++;

		int excessAmount = 0;
		int totalOutput = numberOfReactionsNeeded * currentReaction.output.count;
		while ((totalOutput - excessAmount) % numOutputRequired != 0) {
			excessAmount++;
		}
		excess[currentReaction.output.name] += excessAmount;

		int total = 0;
		for each(Reagent reagent in currentReaction.reagents) {
			int outputReq = numberOfReactionsNeeded * reagent.count;
			if (reagent.name == "ORE") {
				total += outputReq;
			}
			else {
				Reaction &nextReaction = findReactionByOutput(reactions, reagent.name);
				total += addReagents(excess, reactions, nextReaction, outputReq);
			}
		}
		return total;
	}

public:
	static void run() {
		auto input = Utilities::readFile("input/Day14.txt");
		vector<Reaction> reactions = getReactions(input);
		Reaction &fuelR = findReactionByOutput(reactions, "FUEL");
		map<string, int> excess;

		int ore = addReagents(excess, reactions, fuelR, 1);
		cout << "Ore: " << ore << endl;
	}
};