#pragma once

#include "utils.h"

class Piece {
public:
	Point<int> pos;
	Piece* isPinned = nullptr;

	char type, color, score;
	int moves = 0, promotedAt = 0;
	double posFactors[8][8] {};

	Piece(int x, int y, char type, char color, char scorer);

	Piece(Point<int> point, char type, char color, char score);
};
