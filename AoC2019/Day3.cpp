#include "stdafx.h"
#include "Utilities.cpp"

class Day3 {
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

	class Segment {
	public:
		Point pStart, pEnd;
		int wireLengthToSegment;
		Segment(int x1, int y1, int x2, int y2, int wireLengthToSegment) {
			this->pStart = Point(x1, y1);
			this->pEnd = Point(x2, y2);
			this->wireLengthToSegment = wireLengthToSegment;
		}
		bool isHorizontal() {
			return this->pStart.y == this->pEnd.y;
		}

	};

	static vector<Segment> traceWire(vector<string> wire) {
		int x = 0, y = 0, xDir = 0, yDir = 0, wireLengthAcc = 0;
		vector<Segment> segments;
		for each (string path in wire) {
			char dir = path[0];
			switch (dir) {
			case 'U':
				xDir = 0;
				yDir = 1;
				break;
			case 'D':
				xDir = 0;
				yDir = -1;
				break;
			case 'L':
				xDir = -1;
				yDir = 0;
				break;
			case 'R':
				xDir = 1;
				yDir = 0;
				break;
			}
			int distance = stoi(path.substr(1, string::npos), nullptr, 10);
			int xOld = x;
			int yOld = y;
			int xDistance = xDir * distance;
			int yDistance = yDir * distance;
			x += xDistance;
			y += yDistance;
			
			segments.push_back(Segment(xOld, yOld, x, y, wireLengthAcc));
			wireLengthAcc += abs(xDistance) + abs(yDistance);
		}
		return segments;
	}

	static int getCombinedWireLengthAtCollisionPoint(vector<Point> &collisions, Segment line1, Segment line2) {
		bool line1_hori = line1.isHorizontal();
		if (line1_hori == line2.isHorizontal())
			return -1;
		Segment hori = line1_hori ? line1 : line2;
		Segment vert = line1_hori ? line2 : line1;
		if ((hori.pStart.x > vert.pStart.x && hori.pEnd.x > vert.pStart.x) || (hori.pStart.x < vert.pStart.x && hori.pEnd.x < vert.pStart.x) ||
			(hori.pStart.y > vert.pStart.y && hori.pStart.y > vert.pEnd.y) || (hori.pStart.y < vert.pStart.y && hori.pStart.y < vert.pEnd.y))
			return -1;
		int xColl = vert.pStart.x;
		int yColl = hori.pStart.y;
		collisions.push_back(Point(xColl, yColl));

		int steps_hori = hori.wireLengthToSegment + abs(hori.pStart.x - xColl);
		int steps_vert = vert.wireLengthToSegment + abs(vert.pStart.y - yColl);
		return steps_hori + steps_vert;
	}
	
public:
	static void run() {
		vector<string> fileContent = Utilities::readFile("input/Day3.txt");
		vector<string> wire1 = Utilities::splitString(fileContent.at(0), ",");
		vector<string> wire2 = Utilities::splitString(fileContent.at(1), ",");
		vector<Segment> wire1_segments = traceWire(wire1);
		vector<Segment> wire2_segments = traceWire(wire2);

		int minCombinedLength = -1;
		vector<Point> collisions;
		for each (Segment segment1 in wire1_segments) {
			for each (Segment segment2 in wire2_segments) {
				int combinedLength = getCombinedWireLengthAtCollisionPoint(collisions, segment1, segment2);
				if (combinedLength == -1)
					continue;
				if (combinedLength < minCombinedLength || minCombinedLength == -1)
					minCombinedLength = combinedLength;
			}
		}

		int minDistanceFromOrigin = -1;
		for each(Point p in collisions) {
			int distance = p.calcManhattan();
			if (distance < minDistanceFromOrigin || minDistanceFromOrigin == -1)
				minDistanceFromOrigin = distance;
		}

		cout << "Part 1: " << minDistanceFromOrigin << endl;
		cout << "Part 2: " << minCombinedLength << endl;
	}
};