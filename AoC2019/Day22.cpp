#include "stdafx.h"
#include "Utilities.cpp"
#include <regex>
#include <algorithm>

class Day22 {
	static void cut(vector<int>& cards, int num) {
		if (num == 0)
			return;
		auto slice = (num > 0) ? cards.begin() + num : cards.end() + num;
		auto start = vector<int>(cards.begin(), slice);
		auto end = vector<int>(slice, cards.end());
		end.insert(end.end(), start.begin(), start.end());
		cards = end;
	}

	static void dealIncr(vector<int>& cards, int num) {
		vector<int> newCards(cards.size());
		for (int i = 0; i < cards.size(); i++) {
			int newIdx = i * num;
			if (newIdx >= cards.size())
				newIdx = newIdx % cards.size();
			newCards[newIdx] = cards[i];
		}
		cards = newCards;
	}

	static void dealNew(vector<int>& cards) {
		reverse(cards.begin(), cards.end());
	}

public:
	static void run() {
		int deckSize = 10007;
		auto input = Utilities::readFile("input/Day22.txt");
		vector<int> cards(deckSize);
		for (int i = 0; i < cards.size(); i++)
		{
			cards[i] = i;
		}

		regex rxCut("cut (-?\\d+)");
		regex rxDealIncr("deal with increment (-?\\d+)");
		regex rxDealNew("deal into new stack");

		for each(string line in input) {
			if (line == "")
				continue;
			smatch m;
			if (regex_search(line, m, rxCut)) {
				cut(cards, stoi(m[1].str()));
			}
			else if (regex_search(line, m, rxDealIncr)) {
				dealIncr(cards, stoi(m[1].str()));
			} else if (regex_search(line, m, rxDealNew)) {
				dealNew(cards);
			}
			else {
				throw;
			}
		}
		
		for (int i = 0; i < cards.size(); i++)
		{
			if (cards[i] == 2019) {
				cout << "Pt1: " << i << endl;
				break;
			}
		}
		
	}
};