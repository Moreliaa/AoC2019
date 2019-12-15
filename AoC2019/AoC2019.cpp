#include "stdafx.h"
#include <chrono>
#include "Day1.cpp"
#include "Day2.cpp"
#include "Day3.cpp"
#include "Day4.cpp"
#include "Day5.cpp"
#include "Day6.cpp"
#include "Day7.cpp"
#include "Day8.cpp"
#include "Day9.cpp"
#include "Day10.cpp"
#include "Day11.cpp"
#include "Day12.cpp"
#include "Day13.cpp"

int main()
{
	auto start = chrono::system_clock::now();
	//Day1::run();
	//Day2::run();
	//Day3::run();
	//Day4::run();
	//Day5::run();
	//Day6::run();
	//Day7::run();
	//Day8::run();
	//Day9::run();
	//Day10::run();
	//Day11::run();
	//Day12::run();
	Day13::run();
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_time = end - start;
	cout << "Time spent: " << elapsed_time.count() << " seconds" << endl;
	system("pause");
	return 0;
}