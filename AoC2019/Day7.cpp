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
			for (int phase = 0; phase <= amp.maxPhase; phase++)
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

	static vector<int> runAmpsFeedbackMode(vector<Amp> &amps, unsigned idx, vector<int> &input_cache) {

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
		cout << "Maximum Output: " << maximum << endl;

		amps = initAmps(input_cache, 5, 5, 9);
	}
};