#pragma once
#include "stdafx.h"
#include <fstream>

class Utilities {
public:
	static vector<string> readFile(string path) {
		ifstream file;
		vector<string> content;
		file.open(path);
		if (!file.is_open()) {
			cout << "Failed to open file: " << path;
			return content;
		}
		while (!file.eof()) {
			string line;
			getline(file, line);
			content.push_back(line);
		}
		file.close();
		return content;
	};
	static vector<string> splitString(string input, string delimiter) {
		vector<string> result;
		while (true) {
			auto idx = input.find(delimiter);
			if (idx == string::npos) {
				result.push_back(input);
				return result;
			}
			else {
				result.push_back(input.substr(0, idx));
				input.erase(0, idx + delimiter.length());
			}
		}
	}
};