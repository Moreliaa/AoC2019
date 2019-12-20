#include "stdafx.h"
#include "Utilities.cpp"
#include <regex>
#include <map>

class Day14 {
	class Reagent {
	public:
		long long count;
		string name;
		Reagent() {

		}
		Reagent(long long count, string name) {
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

	static Reaction findReactionByOutput(vector<Reaction> &reactions, string name) {
		for (unsigned i = 0; i < reactions.size(); i++)
		{
			if (reactions[i].output.name == name)
			{
				return reactions[i];
			}
		}
		throw;
	}

	static long long addReagents(map<string, long long> &excess, vector<Reaction> &reactions, Reaction &currentReaction, long long numOutputRequired) {
		auto s = excess.find(currentReaction.output.name);
		if (s == excess.end())
			excess[currentReaction.output.name] = 0;

		while (numOutputRequired > 0 && excess[currentReaction.output.name] > 0) {
			numOutputRequired--;
			excess[currentReaction.output.name]--;
		}

		if (numOutputRequired == 0)
			return 0;

		long long added = numOutputRequired % currentReaction.output.count != 0 ? 1 : 0;
		long long numberOfReactionsNeeded = (numOutputRequired / currentReaction.output.count) + added;

		long long totalOutput = numberOfReactionsNeeded * currentReaction.output.count;
		long long excessAmount = totalOutput - numOutputRequired;
		excess[currentReaction.output.name] += excessAmount;

		long long total = 0;
		for each(Reagent reagent in currentReaction.reagents) {
			long long outputReq = numberOfReactionsNeeded * reagent.count;
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
		map<string, long long> excess;
		long long ore = addReagents(excess, reactions, fuelR, 1);
		cout << "Ore: " << ore << endl;
		
		long long targetOre = 1000000000000;
		long long maxRange = 10000000000;
		long long minRange = 1;
		long long next = 500000;
		map<long long, int> attempts;
		auto it = attempts.find(next);
		do  {
			attempts[next] = 1;
			map<string, long long> excess1;
			ore = addReagents(excess1, reactions, fuelR, next);
			cout << "Min: " << minRange << " Max: " << maxRange << " Ore: " << ore << " Fuel: " << next << endl;
			if (ore > targetOre) {
				maxRange = next;
				next = (next + minRange) / 2;
			}
			else if (ore < targetOre) {
				minRange = next;
				next = (maxRange + next) / 2;
			}
			it = attempts.find(next);
		} while (it == attempts.end());
		cout << "Final fuel: " << next << endl;
	}
};