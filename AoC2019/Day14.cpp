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
		int excess = 0;

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

	static int getRequiredOutput(Reaction &current, Reaction &next, Reagent &currentReagent, int numOutputRequired) {
		while (numOutputRequired != 0 && next.excess != 0) {
			numOutputRequired--;
			next.excess--;
		}
		int outputReq = numOutputRequired * currentReagent.count;
		
		int outputProduced = next.output.count;
		if (outputReq % outputProduced != 0) {
			current.excess += (outputReq % outputProduced);
			outputReq = (outputReq / outputProduced) + 1;
		}
		else
			outputReq /= outputProduced;
		return outputReq;
	}

	static int addReagents(map<string, Reaction> &excess, vector<Reaction> &reactions, Reaction &currentReaction, int numOutputRequired) {
		if (currentReaction.reagents.size() == 1 && currentReaction.reagents[0].name == "ORE") {
			while (numOutputRequired != 0 && currentReaction.excess != 0) {
				numOutputRequired--;
				currentReaction.excess--;
			}
			int outputReq = numOutputRequired * currentReaction.reagents[0].count;
			int outputProduced = currentReaction.output.count;
			if (outputReq % outputProduced != 0) {
				currentReaction.excess += (outputReq % outputProduced);
				outputReq = (outputReq / outputProduced) + 1;
			}
			else
				outputReq /= outputProduced;
			return numOutputRequired * currentReaction.reagents[0].count;
		}
		int total = 0;
		for each(Reagent reagent in currentReaction.reagents) {
			Reaction &nextReaction = findReactionByOutput(reactions, reagent.name);
			int outputReq = getRequiredOutput(currentReaction, nextReaction, reagent, numOutputRequired);
			total += addReagents(excess, reactions, nextReaction, outputReq);
		}
		return total;
	}

public:
	static void run() {
		auto input = Utilities::readFile("input/Day14.txt");
		vector<Reaction> reactions = getReactions(input);
		Reaction &fuelR = findReactionByOutput(reactions, "FUEL");
		map<string, Reaction> excess;

		int ore = addReagents(excess, reactions, fuelR, 1);
	}
};