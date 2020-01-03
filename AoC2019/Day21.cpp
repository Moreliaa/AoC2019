#include "stdafx.h"
#include "IntcodeC.cpp"
#include <sstream>
#include "Utilities.cpp"

class Day21 {
public:
	static void run() {
		auto input_cache = IntcodeC::getInput("input/Day21.txt");
		IntcodeProgram prog(input_cache);
		// AND -> Y = X && Y
		// OR -> Y = X || Y
		// NOT -> Y = !X

		/* Pt1:
		NOT A J
		NOT C T
		AND D T
		OR T J
		WALK
		*/

		vector<string> commands = Utilities::readFile("input/Day21command.txt");
		stringstream s_io;
		for (unsigned i = 0; i < commands.size(); i++)
		{
			string commandString = commands[i];
			for (unsigned i = 0; i <= commandString.size(); i++)
			{
				int chara = (i < commandString.size()) ? commandString[i] : '\n';
				IntcodeC::runProgram(prog, s_io, cout, false, true);
				s_io << chara << endl;
				prog.nextInputAvailable = true;
			}
		}

		IntcodeC::runProgram(prog, s_io, s_io, false, false);
		string line;
		long long result = 0;
		do {
			getline(s_io, line);
			if (line != "") {
				int l = stoi(line);
				result = stol(line);
				cout << (char)l;
			}
		} while (line != "");
		cout << "Pt2: " << result << endl;
	}
};