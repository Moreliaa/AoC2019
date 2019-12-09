#include "stdafx.h"
#include "IntcodeC.cpp"
#include <sstream>
#include <algorithm>

class Day7 {
	class Amp {
	public:
		map<long long, long long> program;
		int minPhase;
		int maxPhase;

		Amp() {
			this->minPhase = 0;
			this->maxPhase = 4;
		}

		Amp(int minPhase, int maxPhase) {
			this->minPhase = minPhase;
			this->maxPhase = maxPhase;
		}

		void initProgram(map<long long, long long> input) {
			this->program = input;
		}
	};

	static vector<Amp> initAmps(int size, int minPhase, int maxPhase) {
		vector<Amp> amps;
		for (int i = 0; i < size; i++)
		{
			amps.push_back(Amp(minPhase, maxPhase));
		}
		return amps;
	}

	static vector<vector<int>> getPhaseCombinations(int minPhase, int maxPhase) {
		vector<vector<int>> phaseCombinations;
		for (int i = minPhase; i <= maxPhase; i++)
		{
			for (int j = minPhase; j <= maxPhase; j++)
			{
				if (j == i)
					continue;
				for (int k = minPhase; k <= maxPhase; k++)
				{
					if (k == j || k == i)
						continue;
					for (int l = minPhase; l <= maxPhase; l++)
					{
						if (l == k || l == j || l == i)
							continue;
						for (int m = minPhase; m <= maxPhase; m++)
						{
							if (m == l || m == k || m == j || m == i)
								continue;
							vector<int> phases;
							phases.push_back(i);
							phases.push_back(j);
							phases.push_back(k);
							phases.push_back(l);
							phases.push_back(m);
							phaseCombinations.push_back(phases);
						}
					}
				}
			}
		}
		return phaseCombinations;
	}

	static vector<long long> runAmps(vector<Amp> &amps, map<long long, long long> &input_cache, bool feedbackMode = false) {
		vector<vector<int>> phaseCombinations = getPhaseCombinations(amps[0].minPhase, amps[0].maxPhase);
		vector<long long> results;
		for each(vector<int> combination in phaseCombinations) {
			for (unsigned i = 0; i < amps.size(); i++)
			{
				Amp &amp = amps[i];
				amp.initProgram(input_cache);
			}
			vector<IntcodeProgram> progs;
			stringstream s_io;
			bool firstLoop = true;

			for (unsigned input_idx = 0; input_idx < amps.size(); input_idx++)
			{
				int phase = combination[input_idx];
				int nextInputI;
				string nextInput = "";
				if (firstLoop && input_idx == 0)
					nextInput = "0";
				else
					getline(s_io, nextInput);
				if (nextInput == "") {
					cout << "Missing input!" << endl;
					break;
				}
				else {
					nextInputI = stoi(nextInput, nullptr, 10);
				}
				
				if (firstLoop)
					s_io << phase << endl;
				s_io << nextInputI << endl;

				if (progs.size() <= input_idx)
					progs.push_back(IntcodeProgram(amps[input_idx].program));

				long long returnCode = IntcodeC::runProgram(progs[input_idx], s_io, s_io, true);
				if (returnCode == 99 && input_idx == amps.size() - 1) {
					getline(s_io, nextInput);
					results.push_back(stoi(nextInput, nullptr, 10));
					break;
				}
				if (input_idx == amps.size() - 1 && feedbackMode) {
					input_idx = -1;
					firstLoop = false;
				}
			}
		}
		return results;
	}

	static void runPart(vector<Amp> &amps, map<long long, long long> &input_cache, bool feedbackMode) {
		long long maximum = 0;
		auto results = runAmps(amps, input_cache, feedbackMode);
		for (unsigned i = 0; i < results.size(); i++)
		{
			maximum = max(maximum, results[i]);
		}
		cout << "Maximum Output: " << maximum << endl;
	}
	
public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day7.txt");
		vector<Amp> amps = initAmps(5, 0, 4);
		runPart(amps, input_cache, false);
		amps = initAmps(5, 5, 9);
		runPart(amps, input_cache, true);
	}
};