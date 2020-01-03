#pragma once
#include "stdafx.h"
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <map>

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

template <class T>
class Grid {
public:
	map<string, T> grid;
	long long xMin = 0;
	long long xMax = 0;
	long long yMin = 0;
	long long yMax = 0;

	Grid<T>() {
		this->grid = map<string, T>();
	}

	void reset() {
		this->grid = map<string, T>();
	}

	static string createKey(long long x, long long y, string additionalIdentifier="") {
		if (additionalIdentifier != "")
			additionalIdentifier = "," + additionalIdentifier;
		return to_string(x) + "," + to_string(y) + additionalIdentifier;
	}

	static Point parsePos(string key) {
		vector<string> xy = Utilities::splitString(key, ",");
		Point p = Point(stoi(xy[0]), stoi(xy[1]));
		return p;
	}

	bool seenPos(long long x, long long y, string additionalIdentifier="") {
		auto it = grid.find(createKey(x, y,additionalIdentifier));
		return it != grid.end();
	}

	T& get(long long x, long long y, string additionalIdentifier="") {
		return grid[createKey(x, y, additionalIdentifier)];
	}

	void put(long long x, long long y, T val, string additionalIdentifier="") {
		grid[createKey(x, y,additionalIdentifier)] = val;
		xMin = min(xMin, x);
		xMax = max(xMax, x);
		yMin = min(yMin, y);
		yMax = max(yMax, y);
	}
};