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
#include "Day14.cpp"
#include "Day15.cpp"
#include "Day16.cpp"
#include "Day17.cpp"
#include "Day18.cpp"
#include "Day19.cpp"
#include "Day20.cpp"
#include "Day21.cpp"
#include "Day22.cpp"

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
	//Day13::run();
	//Day14::run();
	//Day15::run();
	//Day16::run();
	//Day17::run();
	//Day18::run();
	//Day19::run();
	//Day20::run();
	//Day21::run();
	Day22::run();
	auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_time = end - start;
	cout << "Time spent: " << elapsed_time.count() << " seconds" << endl;
	system("pause");
	return 0;
}