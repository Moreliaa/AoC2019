#pragma once
#include "stdafx.h"
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

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

	// stolen from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	// trim from start
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}
};

class Point {
public:
	int x, y;
	Point() {
		this->x = 0;
		this->y = 0;
	}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int calcManhattan() {
		return abs(this->x) + abs(this->y);
	}
};