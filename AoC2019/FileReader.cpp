#include "stdafx.h";
#include <fstream>;

class FileReader {
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
};