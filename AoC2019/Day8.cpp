#include "stdafx.h"
#include "Utilities.cpp"

class Day8 {

	static vector<vector<int>> getLayer(string &input, int width, int height, unsigned &idx) {
		unsigned size = width * height;
		vector<vector<int>> layer;
		vector<int> *currentLine = &vector<int>();
		for (unsigned i = idx; i < size + idx; i++)
		{
			if (i > idx && i % width == 0)
			{
				layer.push_back(*currentLine);
				currentLine = &vector<int>();
			}
			string s = input.substr(i, 1);
			currentLine->push_back(stoi(s));
		}
		layer.push_back(*currentLine);
		idx += size;
		return layer;
	}

	static vector<vector<vector<int>>> getLayers(string &input, int width, int height) {
		vector<vector<vector<int>>> layers;
		unsigned idx = 0;
		while (idx < input.size()) {
			layers.push_back(getLayer(input, width, height, idx));
		}
		return layers;
	}

	static int getCountOfDigitInLayer(vector<vector<int>> &layer, int digit) {
		int count = 0;
		for each(vector<int> line in layer) {
			for (unsigned i = 0; i < line.size(); i++)
			{
				if (line[i] == digit)
					count++;
			}
		}
		return count;
	}

	static vector<vector<int>> getImage(vector<vector<vector<int>>> &layers) {
		vector<vector<int>> &image = layers[0];
		int j = 0;
		for (vector<int> &line : image)
		{
			for (unsigned i = 0; i < line.size(); i++)
			{
				for (unsigned k = 0; k < layers.size(); k++) {
					int color = layers[k][j][i];
					if (color != 2) {
						image[j][i] = color;
						break;
					}
				}
			}
			j++;
		}
		return image;
	}

public:
	static void run() {
		vector<string> input = Utilities::readFile("input/Day8.txt");
		int width = 25;
		int height = 6;
		vector<vector<vector<int>>> layers = getLayers(input[0], width, height);
		int zeroes = -1;
		vector<vector<int>> *targetLayer;
		for (vector<vector<int>> &layer : layers)
		{
			int count = getCountOfDigitInLayer(layer, 0);
			if (zeroes == -1 || count < zeroes) {
				zeroes = count;
				targetLayer = &layer;
			}
		}
		int count1 = getCountOfDigitInLayer(*targetLayer, 1);
		int count2 = getCountOfDigitInLayer(*targetLayer, 2);

		cout << "Part 1: " << count1 * count2 << endl;

		vector<vector<int>> &image = getImage(layers);
		int j = 0;
		for (vector<int> &line : image)
		{
			for (unsigned i = 0; i < line.size(); i++)
			{
				if (image[j][i] == 0)
					cout << " ";
				else
					cout << "#";
			}
			cout << endl;
			j++;
		}
	}
};

