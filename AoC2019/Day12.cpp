#include "stdafx.h"
#include "Utilities.cpp"
#include <map>
#include <algorithm>

class Day12 {
	class Moon {
		void gravityAxis(const long long &axis, const long long &partnerAxis, long long &velocityOnAxis) {
			if (axis > partnerAxis)
				velocityOnAxis--;
			else if (axis < partnerAxis)
				velocityOnAxis++;
		}

		long long calcEpot() {
			return abs(x) + abs(y) + abs(z);
		}

		long long calcEkin() {
			return abs(x_v) + abs(y_v) + abs(z_v);
		}

	public:
		long long x, y, z, x_v, y_v, z_v;
		long long x_init, y_init, z_init;

		Moon(long long x, long long y, long long z) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->x_init = x;
			this->y_init = y;
			this->z_init = z;
			this->x_v = 0;
			this->y_v = 0;
			this->z_v = 0;
		}
		void gravity(Moon &partner) {
			gravityAxis(x, partner.x, x_v);
			gravityAxis(y, partner.y, y_v);
			gravityAxis(z, partner.z, z_v);
		}
		void velocity() {
			x += x_v;
			y += y_v;
			z += z_v;
		}

		long long calcEtotal() {
			return calcEpot() * calcEkin();
		}
		bool isInitialState() {
			return x == x_init && y == y_init && z == z_init && x_v == 0 && y_v == 0 && z_v == 0;
		}

		void reset() {
			this->x = x_init;
			this->y = y_init;
			this->z = z_init;
			this->x_v = 0;
			this->y_v = 0;
			this->z_v = 0;
		}

		void printDebugInfo() {
			cout << "pos=<x=" << x << ", y=" << y << ", z=" << z << ">, vel=<x=" << x_v << ", y=" << y_v << ", z=" << z_v << ">" << " Epot: " << calcEpot() << " Ekin: " << calcEkin() << " Total: " << calcEtotal() << endl;
		}

	};
	
	static void applyGravity(vector<Moon> &moons) {
		auto it1 = moons.begin();
		while (it1 != moons.end()) {
			auto it2 = moons.begin();
			while (it2 != moons.end()) {
				if (it1 != it2)
					it1->gravity(*it2);
				it2++;
			}
			it1++;
		}
	}

	static void applyVelocity(vector<Moon> &moons) {
		auto it = moons.begin();
		while (it != moons.end()) {
			it->velocity();
			it++;
		}
	}

	static void step(vector<Moon> &moons, bool debug=false) {
		applyGravity(moons);
		applyVelocity(moons);
		if (debug) {
			auto it = moons.begin();
			while (it != moons.end()) {
				it->printDebugInfo();
				it++;
			}
		}
	}

	static void stepNtimes(vector<Moon> &moons, int steps, bool debug=false) {
		while (steps > 0) {
			step(moons, debug);
			steps--;
		}
	}

	static vector<long long> stepUntilRepeat(vector<Moon> &moons) {
		long long steps = 0;
		long long stepsX = 0;
		long long stepsY = 0;
		long long stepsZ = 0;

		unsigned initX = 0;
		unsigned initY = 0;
		unsigned initZ = 0;

		while (stepsX == 0 || stepsY == 0 || stepsZ == 0) {
			step(moons);
			steps++;

			auto it = moons.begin();
			while (it != moons.end()) {
				if (it->x == it->x_init && it->x_v == 0)
					initX++;
				if (it->y == it->y_init && it->y_v == 0)
					initY++;
				if (it->z == it->z_init && it->z_v == 0)
					initZ++;
				it++;
			}
			if (stepsX == 0 && initX == moons.size())
				stepsX = steps;
			if (stepsY == 0 && initY == moons.size())
				stepsY = steps;
			if (stepsZ == 0 && initZ == moons.size())
				stepsZ = steps;
			initX = 0;
			initY = 0;
			initZ = 0;
		}
		vector<long long> coords;
		coords.push_back(stepsX);
		coords.push_back(stepsY);
		coords.push_back(stepsZ);
		return coords;
	}

	static long long kgv(long long a, long long b) {
		long long small = min(a, b);
		long long large = max(a, b);
		long long result = small;
		while (result % large != 0)
			result += small;
		return result;
	}

	static vector<long long> getKgvForSteps(vector<long long> &steps) {
		vector<long long> next;
		auto it = steps.begin();
		for (unsigned i = 0; i < steps.size() - 1; i++)
		{
			long long a = steps[i];
			long long b = steps[i + 1];
			next.push_back(kgv(a, b));
		}
		return next;
	}

	static long long calcEtotal(vector<Moon> &moons) {
		long long sum = 0;
		auto it = moons.begin();
		while (it != moons.end()) {
			sum += it->calcEtotal();
			it++;
		}
		return sum;
	}

public:
	static void run() {
		vector<Moon> moons;
		moons.push_back(Moon(5, 4, 4));
		moons.push_back(Moon(-11, -11, -3));
		moons.push_back(Moon(0, 7, 0));
		moons.push_back(Moon(-13, 2, 10));
		stepNtimes(moons, 1000);
		cout << "Pt1: " << calcEtotal(moons) << endl;
		auto it = moons.begin();
		while (it != moons.end()) {
			it->reset();
			it++;
		}
		auto stepsForEachCoord = stepUntilRepeat(moons);
		while (stepsForEachCoord.size() > 1)
			stepsForEachCoord = getKgvForSteps(stepsForEachCoord);
		cout << "Pt2: " << stepsForEachCoord[0] << endl;
	}
};