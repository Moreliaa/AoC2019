#include "stdafx.h"
#include "Utilities.cpp"

class Day12 {
	class Moon {
		int x, y, z, x_v, y_v, z_v;

		void gravityAxis(const int axis, const int partnerAxis, int &velocityOnAxis) {
			if (axis > partnerAxis)
				velocityOnAxis--;
			else if (axis < partnerAxis)
				velocityOnAxis++;
		}

		int calcEpot() {
			return abs(x) + abs(y) + abs(z);
		}

		int calcEkin() {
			return abs(x_v) + abs(y_v) + abs(z_v);
		}

	public:
		Moon(int x, int y, int z) {
			this->x = x;
			this->y = y;
			this->z = z;
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

		int calcEtotal() {
			return calcEpot() * calcEkin();
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

	static void stepNtimes(vector<Moon> &moons, int steps) {
		while (steps > 0) {
			cout << "*";
			step(moons);
			steps--;
		}
		cout << endl;
	}

	static int calcEtotal(vector<Moon> &moons) {
		int sum = 0;
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
		
	}
};