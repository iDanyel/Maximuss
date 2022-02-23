#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

constexpr auto INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

constexpr auto WHITE = 0;
constexpr auto BLACK = 1;

constexpr auto ROOK = 0;
constexpr auto KNIGHT = 1;
constexpr auto BISHOP = 2;
constexpr auto QUEEN = 3;
constexpr auto KING = 4;
constexpr auto PAWN = 5;
constexpr auto NONE = 6;

constexpr auto PAWN_SCORE = 1;

constexpr auto INF = INT_MAX;

typedef char GAME_TYPE;

constexpr auto MIDDLEGAME = 0;
constexpr auto KINGS_AND_PAWNS_ENDGAME = 1;
constexpr auto KINGS_AND_ONE_QUEEN_ENDGAME = 2;

template<typename T> class Point {
public:
	T x, y;

	Point() {
		this->x = 0; this->y = 0;
	}

	Point(T x, T y) {
		this->x = x; this->y = y;
	}

	Point(const Point& point) {
		x = point.x; y = point.y;
	}

	bool operator==(const Point& point) const {
		return x == point.x && y == point.y;
	}

	bool operator!=(const Point& point) {
		return !(*this == point);
	}

	Point& operator=(const Point& point) {
		this->x = point.x; this->y = point.y;
		return *this;
	}

	Point operator+(const Point& point) {
		return Point(x + point.x, y + point.y);
	}

	static double getEuclidianDistance(const Point& p1, const Point& p2) {
		return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	}

	friend ostream& operator<<(ostream& os, const Point& point) {
		os << '(' << point.x << ", " << point.y << ')';
		return os;
	}
};
