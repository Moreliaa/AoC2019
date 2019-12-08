#include "stdafx.h"
#include "IntcodeC.cpp"
#include <sstream>
#include <algorithm>

class Day7 {
	class Result {
	public:
		vector<int> usedPhases;
		int result;

		Result() {
			this->result = 0;
		}

		Result(vector<int> &phases, int result) {
			this->usedPhases = phases;
			this->result = result;
		}
	};

	class Amp {
	public:
		vector<int> program;
		vector<Result> outputs;
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

		void initProgram(vector<int> input) {
			this->program = input;
		}
	};

	static vector<Amp> initAmps(vector<int> input, int size, int minPhase, int maxPhase) {
		vector<Amp> amps;
		for (int i = 0; i < size; i++)
		{
			amps.push_back(Amp(minPhase, maxPhase));
		}
		return amps;
	}

	static void runAmp(vector<Amp> &amps, unsigned idx, vector<int> &input_cache) {
		Amp &amp = amps[idx];
		vector<Result> inputs;
		if (idx == 0)
			inputs.push_back(Result());
		else
			inputs = amps[idx - 1].outputs;
		
			amp.initProgram(input_cache);
		for (unsigned input_idx = 0; input_idx < inputs.size(); input_idx++)
		{
			for (int phase = amp.minPhase; phase <= amp.maxPhase; phase++)
			{
				Result ampInput = inputs[input_idx];
				if (find(ampInput.usedPhases.begin(), ampInput.usedPhases.end(), phase) != ampInput.usedPhases.end())
					continue;
				stringstream s_io;
				s_io << phase << endl;
				s_io << ampInput.result << endl;
				IntcodeC::runProgram(amp.program, s_io, s_io);
				int result;
				s_io >> result;
				ampInput.usedPhases.push_back(phase);
				amp.outputs.push_back(Result(ampInput.usedPhases, result));
			}
		}

		if (idx < amps.size() - 1)
			runAmp(amps, idx + 1, input_cache);

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

	static vector<int> runAmpsFeedbackMode(vector<Amp> &amps, vector<int> &input_cache) {
		vector<vector<int>> phaseCombinations = getPhaseCombinations(amps[0].minPhase, amps[0].maxPhase);
		vector<int> results;
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

				int returnCode = IntcodeC::runProgram(progs[input_idx], s_io, s_io, true);
				if (returnCode == 99 && input_idx == amps.size() - 1) {
					getline(s_io, nextInput);
					results.push_back(stoi(nextInput, nullptr, 10));
					break;
				}

				if (input_idx == amps.size() - 1) {
					input_idx = -1;
					firstLoop = false;
				}
			}
		}
		return results;
	}
	
public:
	static void run() {
		vector<int> input_cache = IntcodeC::getInput("input/Day7.txt");
		vector<Amp> amps = initAmps(input_cache, 5, 0, 4);

		runAmp(amps, 0, input_cache);
		int maximum = 0;
		vector<Result> finalOutputs = amps[amps.size() - 1].outputs;
		for (unsigned i = 0; i < finalOutputs.size(); i++)
		{
			maximum = max(maximum, finalOutputs[i].result);
		}
		cout << "Maximum Output Pt1: " << maximum << endl;

		amps = initAmps(input_cache, 5, 5, 9);
		maximum = 0;
		vector<int>	results = runAmpsFeedbackMode(amps, input_cache);
		for (unsigned i = 0; i < results.size(); i++)
		{
			maximum = max(maximum, results[i]);
		}
		cout << "Maximum Output Pt2: " << maximum << endl;
	}
};