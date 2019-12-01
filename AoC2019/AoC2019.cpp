#include "stdafx.h"
#include <chrono>
#include "Day1.cpp"

int main()
{
	auto start = chrono::system_clock::now();
	Day1::run();
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_time = end - start;
	cout << "Time spent: " << elapsed_time.count() << " seconds" << endl;
	system("pause");
	return 0;
}

