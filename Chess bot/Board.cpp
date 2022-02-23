#include "Board.h"

// Private members
void Board::removePiece(Piece* piece) {
	vector<Piece*>& v = currentPieces[piece->color][piece->type];

	for (int i = 0; i < v.size(); i++)
		if (v[i] == piece) {
			v.erase(v.begin() + i);
			break;
		}

	removedPieces.push_back({moveNo + 1, piece});
}

void Board::changeType(Piece* piece, const char type) {
	vector<Piece*>& v = currentPieces[piece->color][piece->type];

	for (int i = 0; i < v.size(); i++)
		if (v[i] == piece) {
			v.erase(v.begin() + i);
			break;
		}

	if (type == PAWN)
		piece->score = 1;
	else if (type == QUEEN)
		piece->score = 9;
	else if (type == ROOK)
		piece->score = 5;
	else if (type == BISHOP)
		piece->score = 3;
	else
		piece->score = 3;

	piece->type = type;
	setPosFactors(piece);
	currentPieces[piece->color][type].push_back(piece);
}

string Board::pieceTypeToString(Piece* piece) const {
	string s;

	if (piece->type == ROOK)
		s = "R";
	else if (piece->type == KNIGHT)
		s = "N";
	else if (piece->type == BISHOP)
		s = "B";
	else if (piece->type == QUEEN)
		s = "Q";
	else if (piece->type == KING)
		s = "K";
	else
		s = "P";

	if (piece->color == WHITE)
		s += "w";
	else
		s += "b";

	return s;
}

void Board::markPinnedPieces() {
	char enemy = (color == WHITE) ? BLACK : WHITE;
	Point<int> pos = currentPieces[color][KING][0]->pos;

	Piece* attacker;
	Piece* defender;

	// Top
	attacker = nullptr, defender = nullptr;

	for (int i = pos.x - 1; i >= 0; i--) {
		if (!cells[i][pos.y])
			continue;

		if (cells[i][pos.y]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[i][pos.y];
		} else if (cells[i][pos.y]->type == ROOK || cells[i][pos.y]->type == QUEEN) {
			attacker = cells[i][pos.y];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Bottom
	attacker = nullptr, defender = nullptr;

	for (int i = pos.x + 1; i < 8; i++) {
		if (!cells[i][pos.y])
			continue;

		if (cells[i][pos.y]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[i][pos.y];
		} else if (cells[i][pos.y]->type == ROOK || cells[i][pos.y]->type == QUEEN) {
			attacker = cells[i][pos.y];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Left
	attacker = nullptr, defender = nullptr;

	for (int j = pos.y - 1; j >= 0; j--) {
		if (!cells[pos.x][j])
			continue;

		if (cells[pos.x][j]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[pos.x][j];
		} else if (cells[pos.x][j]->type == ROOK || cells[pos.x][j]->type == QUEEN) {
			attacker = cells[pos.x][j];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Right
	attacker = nullptr, defender = nullptr;

	for (int j = pos.y + 1; j < 8; j++) {
		if (!cells[pos.x][j])
			continue;

		if (cells[pos.x][j]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[pos.x][j];
		} else if (cells[pos.x][j]->type == ROOK || cells[pos.x][j]->type == QUEEN) {
			attacker = cells[pos.x][j];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Top left
	attacker = nullptr, defender = nullptr;

	for (int i = pos.x - 1, j = pos.y - 1; i >= 0 && j >= 0; i--, j--) {
		if (!cells[i][j])
			continue;

		if (cells[i][j]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[i][j];
		} else if (cells[i][j]->type == BISHOP || cells[i][j]->type == QUEEN) {
			attacker = cells[i][j];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Top right
	attacker = nullptr, defender = nullptr;

	for (int i = pos.x - 1, j = pos.y + 1; i >= 0 && j < 8; i--, j++) {
		if (!cells[i][j])
			continue;

		if (cells[i][j]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[i][j];
		} else if (cells[i][j]->type == BISHOP || cells[i][j]->type == QUEEN) {
			attacker = cells[i][j];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Bottom left
	attacker = nullptr, defender = nullptr;

	for (int i = pos.x + 1, j = pos.y - 1; i < 8 && j >= 0; i++, j--) {
		if (!cells[i][j])
			continue;

		if (cells[i][j]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[i][j];
		} else if (cells[i][j]->type == BISHOP || cells[i][j]->type == QUEEN) {
			attacker = cells[i][j];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;

	// Bottom right
	attacker = nullptr, defender = nullptr;

	for (int i = pos.x + 1, j = pos.y + 1; i < 8 && j < 8; i++, j++) {
		if (!cells[i][j])
			continue;

		if (cells[i][j]->color == color) {
			if (defender) {
				defender = nullptr;
				break;
			} else
				defender = cells[i][j];
		} else if (cells[i][j]->type == BISHOP || cells[i][j]->type == QUEEN) {
			attacker = cells[i][j];
			break;
		} else
			break;
	}

	if (attacker && defender)
		defender->isPinned = attacker;
}

void Board::unmarkPinnedPieces() {
	for (vector<Piece*>& v : currentPieces[color])
		for (Piece* p : v)
			if (p->isPinned)
				p->isPinned = nullptr;
}

void Board::markBlockingCells() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			blockingCells[i][j] = true;
}

void Board::markBlockingCells(Point<int> checkPos) {
	Point<int> kingPos = currentPieces[color][KING][0]->pos;

	blockingCells[checkPos.x][checkPos.y] = true;

	if (kingPos.y == checkPos.y) {
		if (checkPos.x < kingPos.x) {
			// Up
			for (int i = kingPos.x - 1; i > checkPos.x; i--)
				blockingCells[i][kingPos.y] = true;
		} else {
			// Down
			for (int i = kingPos.x + 1; i < checkPos.x; i++)
				blockingCells[i][kingPos.y] = true;
		}
	} else if (kingPos.x == checkPos.x) {
		if (checkPos.y < kingPos.y) {
			// Left
			for (int j = kingPos.y - 1; j > checkPos.y; j--)
				blockingCells[kingPos.x][j] = true;
		} else {
			// Right
			for (int j = kingPos.y + 1; j < checkPos.y; j++)
				blockingCells[kingPos.x][j] = true;
		}
	} else if (abs(kingPos.x - checkPos.x) == abs(kingPos.y - checkPos.y)) {
		if (checkPos.x < kingPos.x && checkPos.y < kingPos.y) {
			// Top left
			for (int i = kingPos.x - 1, j = kingPos.y - 1; i > checkPos.x && j > checkPos.y; i--, j--)
				blockingCells[i][j] = true;
		} else if (checkPos.x < kingPos.x) {
			// Top right
			for (int i = kingPos.x - 1, j = kingPos.y + 1; i > checkPos.x && j < checkPos.y; i--, j++)
				blockingCells[i][j] = true;
		} else if (checkPos.x > kingPos.x && checkPos.y < kingPos.y) {
			// Bottom left
			for (int i = kingPos.x + 1, j = kingPos.y - 1; i < checkPos.x && j > checkPos.y; i++, j--)
				blockingCells[i][j] = true;
		} else {
			// Bottom right
			for (int i = kingPos.x + 1, j = kingPos.y + 1; i < checkPos.x && j < checkPos.y; i++, j++)
				blockingCells[i][j] = true;
		}
	}
}

void Board::unmarkBlockingCells() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (blockingCells[i][j])
				blockingCells[i][j] = false;
}

void Board::markCellsGuardedByRook(Piece* piece) {
	int x = piece->pos.x, y = piece->pos.y;

	// Up
	for (int i = x - 1; i >= 0; i--) {
		if (cells[i][y] && cells[i][y]->color != piece->color && cells[i][y] != currentPieces[color][KING][0])
			break;

		guardedCells[i][y] = true;

		if (cells[i][y] && cells[i][y]->color == piece->color)
			break;
	}

	// Down
	for (int i = x + 1; i < 8; i++) {
		if (cells[i][y] && cells[i][y]->color != piece->color && cells[i][y] != currentPieces[color][KING][0])
			break;

		guardedCells[i][y] = true;

		if (cells[i][y] && cells[i][y]->color == piece->color)
			break;
	}

	// Left
	for (int j = y - 1; j >= 0; j--) {
		if (cells[x][j] && cells[x][j]->color != piece->color && cells[x][j] != currentPieces[color][KING][0])
			break;

		guardedCells[x][j] = true;

		if (cells[x][j] && cells[x][j]->color == piece->color)
			break;
	}

	// Right
	for (int j = y + 1; j < 8; j++) {
		if (cells[x][j] && cells[x][j]->color != piece->color && cells[x][j] != currentPieces[color][KING][0])
			break;

		guardedCells[x][j] = true;

		if (cells[x][j] && cells[x][j]->color == piece->color)
			break;
	}
}

void Board::markCellsGuardedByKnight(Piece* piece) {
	vector<Point<int>> pos;

	pos.push_back(piece->pos + Point<int>(-2, -1));
	pos.push_back(piece->pos + Point<int>(-2, 1));
	pos.push_back(piece->pos + Point<int>(2, -1));
	pos.push_back(piece->pos + Point<int>(2, 1));

	pos.push_back(piece->pos + Point<int>(-1, -2));
	pos.push_back(piece->pos + Point<int>(-1, 2));
	pos.push_back(piece->pos + Point<int>(1, -2));
	pos.push_back(piece->pos + Point<int>(1, 2));

	for (Point<int>& p : pos)
		if (isInside(p))
			if (!cells[p.x][p.y] || cells[p.x][p.y]->color == piece->color)
				guardedCells[p.x][p.y] = true;
}

void Board::markCellsGuardedByBishop(Piece* piece) {
	Point<int> pos = piece->pos;

	// Top left
	for (int i = pos.x - 1, j = pos.y - 1; i >= 0 && j >= 0; i--, j--) {
		if (cells[i][j] && cells[i][j]->color != piece->color && cells[i][j] != currentPieces[color][KING][0])
			break;

		guardedCells[i][j] = true;

		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;
	}

	// Top right
	for (int i = pos.x - 1, j = pos.y + 1; i >= 0 && j < 8; i--, j++) {
		if (cells[i][j] && cells[i][j]->color != piece->color && cells[i][j] != currentPieces[color][KING][0])
			break;

		guardedCells[i][j] = true;

		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;
	}

	// Bottom left
	for (int i = pos.x + 1, j = pos.y - 1; i < 8 && j >= 0; i++, j--) {
		if (cells[i][j] && cells[i][j]->color != piece->color && cells[i][j] != currentPieces[color][KING][0])
			break;

		guardedCells[i][j] = true;

		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;
	}

	// Bottom right
	for (int i = pos.x + 1, j = pos.y + 1; i < 8 && j < 8; i++, j++) {
		if (cells[i][j] && cells[i][j]->color != piece->color && cells[i][j] != currentPieces[color][KING][0])
			break;

		guardedCells[i][j] = true;

		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;
	}
}

void Board::markCellsGuardedByQueen(Piece* piece) {
	markCellsGuardedByRook(piece);
	markCellsGuardedByBishop(piece);
}

void Board::markCellsGuardedByKing(Piece* piece) {
	vector<Point<int>> pos;

	pos.push_back(piece->pos + Point<int>(-1, 0));
	pos.push_back(piece->pos + Point<int>(-1, 1));
	pos.push_back(piece->pos + Point<int>(0, 1));
	pos.push_back(piece->pos + Point<int>(1, 1));

	pos.push_back(piece->pos + Point<int>(1, 0));
	pos.push_back(piece->pos + Point<int>(1, -1));
	pos.push_back(piece->pos + Point<int>(0, -1));
	pos.push_back(piece->pos + Point<int>(-1, -1));

	for (Point<int>& p : pos)
		if (isInside(p) && (!cells[p.x][p.y] || cells[p.x][p.y]->color == piece->color))
			guardedCells[p.x][p.y] = true;
}

void Board::markCellsGuardedByPawn(Piece* piece) {
	Point<int> pos1, pos2;

	if (piece->color == WHITE) {
		pos1 = piece->pos + Point<int>(-1, -1);
		pos2 = piece->pos + Point<int>(-1, 1);
	} else {
		pos1 = piece->pos + Point<int>(1, -1);
		pos2 = piece->pos + Point<int>(1, 1);
	}

	if (isInside(pos1) && (!cells[pos1.x][pos1.y] || cells[pos1.x][pos1.y]->color == piece->color))
		guardedCells[pos1.x][pos1.y] = true;

	if (isInside(pos2) && (!cells[pos2.x][pos2.y] || cells[pos2.x][pos2.y]->color == piece->color))
		guardedCells[pos2.x][pos2.y] = true;
}

void Board::setPosFactorsForRook(Piece* piece) {
	double posFactors[8][8] {
		{ 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,  0.00},
		{ 0.05, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10,  0.05},
		{-0.05, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, -0.05},
		{-0.05, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, -0.05},
		{-0.05, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, -0.05},
		{-0.05, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, -0.05},
		{-0.05, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, -0.05},
		{ 0.00, 0.00, 0.00, 0.05, 0.05, 0.00, 0.00,  0.00}
	};

	if (piece->color == WHITE) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[i][j];
	} else {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[7 - i][7 - j];
	}
}

void Board::setPosFactorsForKnight(Piece* piece) {
	double posFactors[8][8] {
		{-0.50, -0.40, -0.30, -0.30, -0.30, -0.30, -0.40, -0.50},
		{-0.40, -0.20,  0.00,  0.00,  0.00,  0.00, -0.20, -0.40},
		{-0.30,  0.00,  0.10,  0.15,  0.15,  0.10,  0.00, -0.30},
		{-0.30,  0.05,  0.15,  0.20,  0.20,  0.15,  0.05, -0.30},
		{-0.30,  0.00,  0.15,  0.20,  0.20,  0.15,  0.00, -0.30},
		{-0.30,  0.05,  0.10,  0.15,  0.15,  0.10,  0.05, -0.30},
		{-0.40, -0.20,  0.00,  0.05,  0.05,  0.00, -0.20, -0.40},
		{-0.50, -0.40, -0.30, -0.30, -0.30, -0.30, -0.40, -0.50}
	};

	if (piece->color == WHITE) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[i][j];
	} else {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[7 - i][7 - j];
	}
}

void Board::setPosFactorsForBishop(Piece* piece) {
	double posFactors[8][8] {
		{-0.20, -0.10, -0.10, -0.10, -0.10, -0.10, -0.10, -0.20},
		{-0.10,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00, -0.10},
		{-0.10,  0.00,  0.05,  0.10,  0.10,  0.05,  0.00, -0.10},
		{-0.10,  0.05,  0.05,  0.10,  0.10,  0.05,  0.05, -0.10},
		{-0.10,  0.00,  0.10,  0.10,  0.10,  0.10,  0.00, -0.10},
		{-0.10,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10, -0.10},
		{-0.10,  0.05,  0.00,  0.00,  0.00,  0.00,  0.05, -0.10},
		{-0.20, -0.10, -0.10, -0.10, -0.10, -0.10, -0.10, -0.20}
	};

	if (piece->color == WHITE) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[i][j];
	} else {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[7 - i][7 - j];
	}
}

void Board::setPosFactorsForQueen(Piece* piece) {
	double posFactors[8][8] {
		{-0.20, -0.10, -0.10, -0.05, -0.05, -0.10, -0.10, -0.20},
		{-0.10,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00, -0.10},
		{-0.10,  0.00,  0.05,  0.05,  0.05,  0.05,  0.00,  0.10},
		{-0.05,  0.00,  0.05,  0.05,  0.05,  0.05,  0.00, -0.05},
		{ 0.00,  0.00,  0.05,  0.05,  0.05,  0.05,  0.00, -0.05},
		{-0.10,  0.05,  0.05,  0.05,  0.05,  0.05,  0.00, -0.10},
		{-0.10,  0.00,  0.05,  0.00,  0.00,  0.00,  0.00, -0.10},
		{-0.20, -0.10, -0.10, -0.05, -0.05, -0.10, -0.10, -0.20}
	};

	if (piece->color == WHITE) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[i][j];
	} else {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[7 - i][7 - j];
	}
}

void Board::setPosFactorsForKing(Piece* piece) {
	double posFactors[8][8] {
		{-0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30},
		{-0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30},
		{-0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30},
		{-0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30},
		{-0.20, -0.30, -0.30, -0.40, -0.40, -0.30, -0.30, -0.20},
		{-0.10, -0.20, -0.20, -0.20, -0.20, -0.20, -0.20, -0.10},
		{ 0.20,  0.20,  0.00,  0.00,  0.00,  0.00,  0.20,  0.20},
		{ 0.20,  0.30,  0.10,  0.00,  0.00,  0.10,  0.30,  0.20}
	};

	if (piece->color == WHITE) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[i][j];
	} else {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[7 - i][7 - j];
	}
}

void Board::setPosFactorsForPawn(Piece* piece) {
	double posFactors[8][8]{
		{0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00, 0.00},
		{0.50,  0.50,  0.50,  0.50,  0.50,  0.50,  0.50, 0.50},
		{0.10,  0.10,  0.20,  0.30,  0.30,  0.20,  0.10, 0.10},
		{0.05,  0.05,  0.10,  0.25,  0.25,  0.10,  0.05, 0.05},
		{0.00,  0.00,  0.00,  0.20,  0.20,  0.00,  0.00, 0.00},
		{0.05, -0.05, -0.10,  0.00,  0.00, -0.10, -0.05, 0.05},
		{0.05,  0.10,  0.10, -0.20, -0.20,  0.10,  0.10, 0.05},
		{0.00,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00, 0.00}
	};

	if (piece->color == WHITE) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[i][j];
	} else {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				piece->posFactors[i][j] = posFactors[7 - i][7 - j];
	}
}

bool Board::canEnPassant(Piece* const piece, const char dir) const {
	Point<int> pos = piece->pos + (dir == 'l' ? Point<int>(0, -1) : Point<int>(0, 1));
	Point<int> dest = pos + (piece->color == WHITE ? Point<int>(-1, 0) : Point<int>(1, 0));

	if (!isInside(pos) || !isInside(dest))
		return false;

	if (!blockingCells[pos.x][pos.y] && !blockingCells[dest.x][dest.y])
		return false;

	if (cells[pos.x][pos.y] && cells[pos.x][pos.y]->type == PAWN && cells[pos.x][pos.y]->color != piece->color)
		if (prevMoves.back().move.second == pos && abs(prevMoves.back().move.first.x - pos.x) == 2) {
			Piece* piece1 = nullptr;
			Piece* piece2 = nullptr;

			if (dir == 'l') {
				for (int j = pos.y - 1; j >= 0; j--)
					if (cells[pos.x][j]) {
						piece1 = cells[pos.x][j];
						break;
					}

				for (int j = piece->pos.y + 1; j < 8; j++)
					if (cells[piece->pos.x][j]) {
						piece2 = cells[piece->pos.x][j];
						break;
					}
			} else {
				for (int j = piece->pos.y - 1; j >= 0; j--)
					if (cells[piece->pos.x][j]) {
						piece1 = cells[piece->pos.x][j];
						break;
					}

				for (int j = pos.y + 1; j < 8; j++)
					if (cells[pos.x][j]) {
						piece2 = cells[pos.x][j];
						break;
					}
			}

			if (!piece1 || !piece2)
				return true;

			if (piece1->type == KING && (piece2->type == ROOK || piece2->type == QUEEN) &&
				piece1->color == color && piece2->color != piece1->color)
					return false;

			if (piece2->type == KING && (piece1->type == ROOK || piece1->type == QUEEN) &&
				piece2->color == color && piece1->color != piece2->color)
					return false;

			return true;
		}

	return false;
}

// Public members
Board::Board() : Board(INITIAL_FEN) {

}

Board::Board(int) : cells(8), currentPieces(2) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			guardedCells[i][j] = false;
			blockingCells[i][j] = false;
		}

	for (vector<Piece*>& cell : cells)
		cell.resize(8);

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			cells[i][j] = nullptr;

	currentPieces[WHITE].resize(6);
	currentPieces[BLACK].resize(6);
}

Board::Board(const string fen) : cells(8), currentPieces(2) {
	int i = 0, line = 0, column = 0;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			guardedCells[i][j] = false;
			blockingCells[i][j] = false;
		}

	for (vector<Piece*>& cell : cells)
		cell.resize(8);

	currentPieces[WHITE].resize(6);
	currentPieces[BLACK].resize(6);

	// Arrangement of the pieces
	for (; fen[i] != ' '; i++)
		if (fen[i] == '/')
			line++, column = 0;
		else if (fen[i] >= '1' && fen[i] <= '8') {
			for (int j = 0; j < fen[i] - '0'; j++)
				cells[line][column++] == nullptr;
		} else {
			Piece* piece = new Piece(line, column, NONE, NONE, NONE);

			if (islower(fen[i]))
				piece->color = BLACK;
			else
				piece->color = WHITE;

			char type, score;

			if (fen[i] == 'r' || fen[i] == 'R')
				type = ROOK, score = 5;
			else if (fen[i] == 'b' || fen[i] == 'B')
				type = BISHOP, score = 3;
			else if (fen[i] == 'n' || fen[i] == 'N')
				type = KNIGHT, score = 3;
			else if (fen[i] == 'q' || fen[i] == 'Q')
				type = QUEEN, score = 9;
			else if (fen[i] == 'k' || fen[i] == 'K')
				type = KING, score = 0;
			else {
				type = PAWN, score = 1;

				if ((piece->color == WHITE && piece->pos.x != 6) ||
					(piece->color == BLACK && piece->pos.x != 1))
						piece->moves++;
			}
		
			piece->type = type; piece->score = score;
			currentPieces[piece->color][piece->type].push_back(piece);
			setPosFactors(piece);

			cells[line][column++] = piece;
		}

	// Who's moving
	color = fen[i + 1] == 'w' ? WHITE : BLACK;

	// Castle availability
	currentPieces[WHITE][KING][0]->moves++;
	currentPieces[BLACK][KING][0]->moves++;

	for (Piece* p : currentPieces[WHITE][ROOK])
		p->moves++;

	for (Piece* p : currentPieces[BLACK][ROOK])
		p->moves++;

	if (fen[i + 3] != '-') {
		i += 3;

		for (; fen[i] != ' '; i++)
			if (fen[i] == 'K') {
				currentPieces[WHITE][KING][0]->moves = 0;
				cells[7][7]->moves--;
			} else if (fen[i] == 'Q') {
				currentPieces[WHITE][KING][0]->moves = 0;
				cells[7][0]->moves--;
			} else if (fen[i] == 'k') {
				currentPieces[BLACK][KING][0]->moves = 0;
				cells[0][7]->moves--;
			} else {
				currentPieces[BLACK][KING][0]->moves = 0;
				cells[0][0]->moves--;
			}

		i++;
	} else
		i += 5;

	// Pawn has just made a two-square move
	if (fen[i] != '-') {
		string src(fen, i, 1), dest(fen, i, 2);

		if (color == WHITE)
			src += dest[1] + 2;
		else
			src += dest[1] - 2;

		prevMoves.push_back({{stringToCoordinates(src), stringToCoordinates(dest)}, getPositionKey()}), i += 3;
	} else
		i += 2;

	// Halfmove clock
	for (; fen[i] != ' '; i++)
		halfmoveClock = halfmoveClock * 10 + (fen[i] - '0');

	// Calculate the move number from the fullmove number
	for (i++; i < fen.size(); i++)
		moveNo = moveNo * 10 + (fen[i] - '0');

	moveNo = (moveNo - 1) * 2;

	if (color == BLACK)
		moveNo++;
}

Board::~Board() {
	for (vector<Piece*> v : currentPieces[WHITE])
		for (Piece* p : v)
			delete p;

	for (vector<Piece*> v : currentPieces[BLACK])
		for (Piece* p : v)
			delete p;
}

string Board::getFen() {
	string fen;

	// Arrangement of the pieces
	for (int i = 0; i < 8; i++) {
		int no = 0, column = 0;

		while (column < 8) {
			while (column < 8 && !cells[i][column])
				column++, no++;

			if (no)
				fen.push_back(no + '0'), no = 0;

			while (column < 8 && cells[i][column]) {
				char c;

				if (cells[i][column]->type == ROOK)
					c = 'r';
				else if (cells[i][column]->type == KNIGHT)
					c = 'n';
				else if (cells[i][column]->type == BISHOP)
					c = 'b';
				else if (cells[i][column]->type == QUEEN)
					c = 'q';
				else if (cells[i][column]->type == KING)
					c = 'k';
				else
					c = 'p';
					
				c = cells[i][column]->color == WHITE ? toupper(c) : c;
				fen.push_back(c);
				column++;
			}
		}

		if (i != 7)
			fen.push_back('/');
	}

	// Who's moving
	fen.push_back(' ');
	fen.push_back(color == WHITE ? 'w' : 'b');

	// Castle availability
	fen.push_back(' ');

	if (!currentPieces[WHITE][KING][0]->moves) {
		for (Piece* p : currentPieces[WHITE][ROOK])
			if (!p->moves) {
				if (p->pos.y == 7) {
					if (fen.back() == 'Q') {
						fen.back() = 'K';
						fen.push_back('Q');
					} else
						fen.push_back('K');
				} else
					fen.push_back('Q');
			}
	}
	
	if (!currentPieces[BLACK][KING][0]->moves) {
		for (Piece* p : currentPieces[BLACK][ROOK])
			if (!p->moves) {
				if (p->pos.y == 7) {
					if (fen.back() == 'q') {
						fen.back() = 'k';
						fen.push_back('q');
					} else
						fen.push_back('k');
				} else
					fen.push_back('q');
			}
	}

	if (fen.back() == ' ')
		fen.push_back('-');

	// Pawn has just made a two-square move
	fen.push_back(' ');

	if (prevMoves.size()) {
		Point<int> pos1 = prevMoves.back().move.first;
		Point<int> pos2 = prevMoves.back().move.second;

		if (cells[pos2.x][pos2.y]->type == PAWN && abs(pos1.x - pos2.x) == 2) {
			Point<int> pos3 = pos2 + Point<int>(0, -1);
			Point<int> pos4 = pos2 + Point<int>(0, 1);

			if ((isInside(pos3) && cells[pos3.x][pos3.y]->type == PAWN && cells[pos3.x][pos3.y]->color != cells[pos2.x][pos2.y]->color) ||
				(isInside(pos4) && cells[pos4.x][pos4.y]->type == PAWN && cells[pos4.x][pos4.y]->color != cells[pos2.x][pos2.y]->color))
					fen += coordinatesToString(pos2);
			else
				fen.push_back('-');
		} else
			fen.push_back('-');
	} else
		fen.push_back('-');

	// Halfmove clock
	fen.push_back(' ');
	fen += to_string(halfmoveClock);

	// Calculate the move number from the fullmove number
	fen.push_back(' ');
	fen += to_string((color == BLACK ? moveNo - 1 : moveNo) / 2 + 1);

	return fen;
}

string Board::getPositionKey() {
	string posKey;

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (!cells[i][j])
				posKey.push_back(NONE);
			else
				posKey.push_back((cells[i][j]->type - '0') * 10 + cells[i][j]->color - '0');

	return posKey;
}

void Board::setPosFactors(Piece* piece) {
	if (piece->type == ROOK)
		setPosFactorsForRook(piece);
	else if (piece->type == BISHOP)
		setPosFactorsForBishop(piece);
	else if (piece->type == KNIGHT)
		setPosFactorsForKnight(piece);
	else if (piece->type == QUEEN)
		setPosFactorsForQueen(piece);
	else if (piece->type == KING)
		setPosFactorsForKing(piece);
	else
		setPosFactorsForPawn(piece);
}

bool Board::isIncheck() {
	vector<Point<int>> checkPos;

	guardedBy(checkPos, currentPieces[color][KING][0]->pos, color == WHITE ? BLACK : WHITE);
	return checkPos.size();
}

bool Board::isInside(Point<int> pos) const {
	return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

void Board::guardedBy(vector<Point<int>>& guards, Point<int> pos, const char color, bool all) {
	// Guarded by knight
	for (int i = 0; i < currentPieces[color][KNIGHT].size(); i++) {
		Point<int> pos1 = currentPieces[color][KNIGHT][i]->pos;

		int diff1 = abs(pos.x - pos1.x);
		int diff2 = abs(pos.y - pos1.y);

		if ((diff1 == 1 && diff2 == 2) || (diff1 == 2 && diff2 == 1)) {
			guards.push_back(Point<int>(pos1));

			if (!all)
				return;
		}
	}

	// Guarded by pawn
	{
		Point<int> pos1, pos2;

		if (color == WHITE) {
			pos1 = Point<int>(pos.x + 1, pos.y - 1);
			pos2 = Point<int>(pos.x + 1, pos.y + 1);
		} else {
			pos1 = Point<int>(pos.x - 1, pos.y - 1);
			pos2 = Point<int>(pos.x - 1, pos.y + 1);
		}

		if (isInside(pos1) && cells[pos1.x][pos1.y])
			if (cells[pos1.x][pos1.y]->type == PAWN)
				if (cells[pos1.x][pos1.y]->color == color) {
					guards.push_back(pos1);

					if (!all)
						return;
				}

		if (isInside(pos2) && cells[pos2.x][pos2.y])
			if (cells[pos2.x][pos2.y]->type == PAWN)
				if (cells[pos2.x][pos2.y]->color == color) {
					guards.push_back(pos2);

					if (!all)
						return;
				}
	}

	// Guarded by rook or queen
	for (int i = pos.x - 1; i >= 0; i--)
		if (cells[i][pos.y]) {
			if (cells[i][pos.y]->color != color)
				break;
			else if (cells[i][pos.y]->type != ROOK && cells[i][pos.y]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(i, pos.y));

				if (!all)
					return;

				break;
			}
		}

	for (int i = pos.x + 1; i < 8; i++)
		if (cells[i][pos.y]) {
			if (cells[i][pos.y]->color != color)
				break;
			else if (cells[i][pos.y]->type != ROOK && cells[i][pos.y]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(i, pos.y));

				if (!all)
					return;

				break;
			}
		}

	for (int j = pos.y - 1; j >= 0; j--)
		if (cells[pos.x][j]) {
			if (cells[pos.x][j]->color != color)
				break;
			else if (cells[pos.x][j]->type != ROOK && cells[pos.x][j]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(pos.x, j));

				if (!all)
					return;

				break;
			}
		}

	for (int j = pos.y + 1; j < 8; j++)
		if (cells[pos.x][j]) {
			if (cells[pos.x][j]->color != color)
				break;
			else if (cells[pos.x][j]->type != ROOK && cells[pos.x][j]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(pos.x, j));

				if (!all)
					return;

				break;
			}
		}

	// Guarded by bishop or queen
	for (int i = pos.x - 1, j = pos.y - 1; i >= 0 && j >= 0; i--, j--)
		if (cells[i][j]) {
			if (cells[i][j]->color != color)
				break;
			else if (cells[i][j]->type != BISHOP && cells[i][j]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(i, j));

				if (!all)
					return;

				break;
			}
		}

	for (int i = pos.x - 1, j = pos.y + 1; i >= 0 && j < 8; i--, j++)
		if (cells[i][j]) {
			if (cells[i][j]->color != color)
				break;
			else if (cells[i][j]->type != BISHOP && cells[i][j]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(i, j));

				if (!all)
					return;

				break;
			}
		}

	for (int i = pos.x + 1, j = pos.y - 1; i < 8 && j >= 0; i++, j--)
		if (cells[i][j]) {
			if (cells[i][j]->color != color)
				break;
			else if (cells[i][j]->type != BISHOP && cells[i][j]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(i, j));

				if (!all)
					return;

				break;
			}
		}

	for (int i = pos.x + 1, j = pos.y + 1; i < 8 && j < 8; i++, j++)
		if (cells[i][j]) {
			if (cells[i][j]->color != color)
				break;
			else if (cells[i][j]->type != BISHOP && cells[i][j]->type != QUEEN)
				break;
			else {
				guards.push_back(Point<int>(i, j));

				if (!all)
					return;

				break;
			}
		}
}

void Board::markGuardedCells(const char color) {
	for (Piece* p : currentPieces[color][ROOK])
		markCellsGuardedByRook(p);

	for (Piece* p : currentPieces[color][KNIGHT])
		markCellsGuardedByKnight(p);

	for (Piece* p : currentPieces[color][BISHOP])
		markCellsGuardedByBishop(p);

	for (Piece* p : currentPieces[color][QUEEN])
		markCellsGuardedByQueen(p);

	markCellsGuardedByKing(currentPieces[color][KING][0]);

	for (Piece* p : currentPieces[color][PAWN])
		markCellsGuardedByPawn(p);
}

void Board::unmarkGuardedCells() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (guardedCells[i][j])
				guardedCells[i][j] = false;
}

void Board::findPossibleMoves(vector<string>& possibleMoves) {
	vector<Point<int>> checkPos;
	
	guardedBy(checkPos, currentPieces[color][KING][0]->pos, color == WHITE ? BLACK : WHITE, true);

	// King is in check(it could be double check)
	if (checkPos.size() >= 1) {
		findPossibleMovesForKing(currentPieces[color][KING][0], possibleMoves, true);

		if (checkPos.size() == 1) {
			markPinnedPieces();
			markBlockingCells(checkPos.front());

			for (Piece* p : currentPieces[color][ROOK])
				findPossibleMovesForRook(p, possibleMoves);
			
			for (Piece* p : currentPieces[color][KNIGHT])
				findPossibleMovesForKnight(p, possibleMoves);
			
			for (Piece* p : currentPieces[color][BISHOP])
				findPossibleMovesForBishop(p, possibleMoves);
			
			for (Piece* p : currentPieces[color][QUEEN])
				findPossibleMovesForQueen(p, possibleMoves);
			
			for (Piece* p : currentPieces[color][PAWN])
				findPossibleMovesForPawn(p, possibleMoves);
			
			unmarkBlockingCells();
			unmarkPinnedPieces();
		}
		
		return;
	}

	// King is not in check
	findPossibleMovesForKing(currentPieces[color][KING][0], possibleMoves);
	markPinnedPieces();
	markBlockingCells();

	for (Piece* p : currentPieces[color][ROOK])
		findPossibleMovesForRook(p, possibleMoves);

	for (Piece* p : currentPieces[color][KNIGHT])
		findPossibleMovesForKnight(p, possibleMoves);

	for (Piece* p : currentPieces[color][BISHOP])
		findPossibleMovesForBishop(p, possibleMoves);

	for (Piece* p : currentPieces[color][QUEEN])
		findPossibleMovesForQueen(p, possibleMoves);

	for (Piece* p : currentPieces[color][PAWN])
		findPossibleMovesForPawn(p, possibleMoves);

	unmarkPinnedPieces();
	unmarkBlockingCells();
}

void Board::findPossibleMovesForRook(Piece* piece, vector<string>& possibleMoves) {
	if (piece->isPinned) {
		Point<int> pos1 = currentPieces[color][KING][0]->pos;
		Point<int> pos2 = piece->isPinned->pos;

		// Diagonal pin
		if (abs(pos1.x - pos2.x) == abs(pos1.y - pos2.y))
			return;

		if (pos1.y == pos2.y) {
			if (pos2.x < pos1.x) {
				// Top pin
				for (int i = pos2.x; i < pos1.x; i++)
					if (Point<int>(i, pos1.y) != piece->pos)
						if (blockingCells[i][pos1.y])
							possibleMoves.push_back(createMove(piece->pos, {i, pos1.y}));
			} else {
				// Bottom pin
				for (int i = pos1.x + 1; i <= pos2.x; i++)
					if (Point<int>(i, pos1.y) != piece->pos)
						if (blockingCells[i][pos1.y])
							possibleMoves.push_back(createMove(piece->pos, {i, pos1.y}));
			}
		} else {
			if (pos2.y < pos1.y) {
				// Left pin
				for (int j = pos2.y; j < pos1.y; j++)
					if (Point<int>(pos1.x, j) != piece->pos)
						if (blockingCells[pos1.x][j])
							possibleMoves.push_back(createMove(piece->pos, {pos1.x, j}));
			} else {
				// Right pin
				for (int j = pos1.y + 1; j <= pos2.y; j++)
					if (Point<int>(pos1.x, j) != piece->pos)
						if (blockingCells[pos1.x][j])
							possibleMoves.push_back(createMove(piece->pos, {pos1.x, j}));
			}
		}

		return;
	}

	int x = piece->pos.x, y = piece->pos.y;

	// Up
	for (int i = x - 1; i >= 0; i--) {
		if (cells[i][y] && cells[i][y]->color == piece->color)
			break;

		if (blockingCells[i][y])
			possibleMoves.push_back(createMove(piece->pos, {i, y}));

		if (cells[i][y])
			break;
	}

	// Down
	for (int i = x + 1; i < 8; i++) {
		if (cells[i][y] && cells[i][y]->color == piece->color)
			break;

		if (blockingCells[i][y])
			possibleMoves.push_back(createMove(piece->pos, {i, y}));

		if (cells[i][y])
			break;
	}

	// Left
	for (int j = y - 1; j >= 0; j--) {
		if (cells[x][j] && cells[x][j]->color == piece->color)
			break;

		if (blockingCells[x][j])
			possibleMoves.push_back(createMove(piece->pos, {x, j}));

		if (cells[x][j])
			break;
	}

	// Right
	for (int j = y + 1; j < 8; j++) {
		if (cells[x][j] && cells[x][j]->color == piece->color)
			break;

		if (blockingCells[x][j])
			possibleMoves.push_back(createMove(piece->pos, { x, j }));

		if (cells[x][j])
			break;
	}
}

void Board::findPossibleMovesForKnight(Piece* piece, vector<string>& possibleMoves) {
	if (piece->isPinned)
		return;

	vector<Point<int>> pos;

	pos.push_back(piece->pos + Point<int>(-2, -1));
	pos.push_back(piece->pos + Point<int>(-2, 1));
	pos.push_back(piece->pos + Point<int>(2, -1));
	pos.push_back(piece->pos + Point<int>(2, 1));

	pos.push_back(piece->pos + Point<int>(-1, -2));
	pos.push_back(piece->pos + Point<int>(-1, 2));
	pos.push_back(piece->pos + Point<int>(1, -2));
	pos.push_back(piece->pos + Point<int>(1, 2));

	for (Point<int>& p : pos)
		if (isInside(p))
			if (!cells[p.x][p.y] || cells[p.x][p.y]->color != piece->color)
				if (blockingCells[p.x][p.y])
					possibleMoves.push_back(createMove(piece->pos, p));
}

void Board::findPossibleMovesForBishop(Piece* piece, vector<string>& possibleMoves) {
	if (piece->isPinned) {
		Point<int> pos1 = currentPieces[color][KING][0]->pos;
		Point<int> pos2 = piece->isPinned->pos;

		// Line pin or column pin
		if (pos1.x == pos2.x || pos1.y == pos2.y)
			return;

		if (pos2.x < pos1.x && pos2.y < pos1.y) {
			// Top left pin
			for (int i = pos1.x - 1, j = pos1.y - 1; i >= pos2.x && j >= pos2.y; i--, j--)
				if (Point<int>(i, j) != piece->pos)
					if (blockingCells[i][j])
						possibleMoves.push_back(createMove(piece->pos, {i, j}));
		} else if (pos2.x < pos1.x) {
			// Top right pin
			for (int i = pos1.x - 1, j = pos1.y + 1; i >= pos2.x && j <= pos2.y; i--, j++)
				if (Point<int>(i, j) != piece->pos)
					if (blockingCells[i][j])
						possibleMoves.push_back(createMove(piece->pos, {i, j}));
		} else if (pos2.y < pos1.y) {
			// Bottom left pin
			for (int i = pos1.x + 1, j = pos1.y - 1; i <= pos2.x && j >= pos2.y; i++, j--)
				if (Point<int>(i, j) != piece->pos)
					if (blockingCells[i][j])
						possibleMoves.push_back(createMove(piece->pos, {i, j}));
		} else {
			// Bottom right pin
			for (int i = pos1.x + 1, j = pos1.y + 1; i <= pos2.x && j <= pos2.y; i++, j++)
				if (Point<int>(i, j) != piece->pos)
					if (blockingCells[i][j])
						possibleMoves.push_back(createMove(piece->pos, {i, j}));
		}

		return;
	}

	Point<int> pos = piece->pos;

	// Top left
	for (int i = pos.x - 1, j = pos.y - 1; i >= 0 && j >= 0; i--, j--) {
		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;

		if (blockingCells[i][j])
			possibleMoves.push_back(createMove(piece->pos, {i, j}));

		if (cells[i][j])
			break;
	}

	// Top right
	for (int i = pos.x - 1, j = pos.y + 1; i >= 0 && j < 8; i--, j++) {
		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;

		if (blockingCells[i][j])
			possibleMoves.push_back(createMove(piece->pos, {i, j}));

		if (cells[i][j])
			break;
	}

	// Bottom left
	for (int i = pos.x + 1, j = pos.y - 1; i < 8 && j >= 0; i++, j--) {
		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;

		if (blockingCells[i][j])
			possibleMoves.push_back(createMove(piece->pos, {i, j}));

		if (cells[i][j])
			break;
	}

	// Bottom right
	for (int i = pos.x + 1, j = pos.y + 1; i < 8 && j < 8; i++, j++) {
		if (cells[i][j] && cells[i][j]->color == piece->color)
			break;

		if (blockingCells[i][j])
			possibleMoves.push_back(createMove(piece->pos, {i, j}));

		if (cells[i][j])
			break;
	}
}

void Board::findPossibleMovesForQueen(Piece* piece, vector<string>& possibleMoves) {
	findPossibleMovesForRook(piece, possibleMoves);
	findPossibleMovesForBishop(piece, possibleMoves);
}

void Board::findPossibleMovesForKing(Piece* piece, vector<string>& possibleMoves, bool inCheck) {
	vector<Point<int>> pos;

	pos.push_back(piece->pos + Point<int>(-1, 0));
	pos.push_back(piece->pos + Point<int>(-1, 1));
	pos.push_back(piece->pos + Point<int>(0, 1));
	pos.push_back(piece->pos + Point<int>(1, 1));

	pos.push_back(piece->pos + Point<int>(1, 0));
	pos.push_back(piece->pos + Point<int>(1, -1));
	pos.push_back(piece->pos + Point<int>(0, -1));
	pos.push_back(piece->pos + Point<int>(-1, -1));

	markGuardedCells(color == WHITE ? BLACK : WHITE);

	for (Point<int>& p : pos)
		if (isInside(p) && !guardedCells[p.x][p.y])
			if (!cells[p.x][p.y] || cells[p.x][p.y]->color != piece->color)
				possibleMoves.push_back(createMove(piece->pos, p));

	if (!inCheck && !piece->moves) {
		Point<int> pos1, pos2;

		if (piece->color == WHITE) {
			pos1 = Point<int>(7, 0);
			pos2 = Point<int>(7, 7);
		} else {
			pos1 = Point<int>(0, 0);
			pos2 = Point<int>(0, 7);
		}

		// Big castle
		if (cells[pos1.x][pos1.y] && !cells[pos1.x][pos1.y]->moves)
			if (!cells[pos1.x][1] && !cells[pos1.x][2] && !cells[pos1.x][3])
				if (!guardedCells[pos1.x][2] && !guardedCells[pos1.x][3])
					possibleMoves.push_back(createMove(piece->pos, piece->pos + Point<int>(0, -2)));

		// Small castle
		if (cells[pos2.x][pos2.y] && !cells[pos2.x][pos2.y]->moves)
			if (!cells[pos2.x][6] && !cells[pos2.x][5])
				if (!guardedCells[pos1.x][6] && !guardedCells[pos1.x][5])
					possibleMoves.push_back(createMove(piece->pos, piece->pos + Point<int>(0, 2)));
	}

	unmarkGuardedCells();
}

void Board::findPossibleMovesForPawn(Piece* piece, vector<string>& possibleMoves) {
	if (piece->isPinned) {
		Point<int> pos1 = currentPieces[color][KING][0]->pos;
		Point<int> pos2 = piece->isPinned->pos;

		// Line pin
		if (pos1.x == pos2.x)
			return;

		// Diagonal pin
		if (abs(pos1.x - pos2.x) == abs(pos1.y - pos2.y)) {
			Point<int> pos3, pos4;

			if (piece->color == WHITE) {
				pos3 = piece->pos + Point<int>(-1, -1);
				pos4 = piece->pos + Point<int>(-1, 1);
			} else {
				pos3 = piece->pos + Point<int>(1, -1);
				pos4 = piece->pos + Point<int>(1, 1);
			}

			if (abs(pos1.x - pos3.x) == abs(pos1.y - pos3.y)) {
				if (cells[pos3.x][pos3.y] && cells[pos3.x][pos3.y]->color != piece->color)
					if (blockingCells[pos3.x][pos3.y]) {
						if (pos3.x == 0 || pos3.x == 7) {
							string move = createMove(piece->pos, pos3);

							possibleMoves.push_back(move + 'q');
							possibleMoves.push_back(move + 'r');
							possibleMoves.push_back(move + 'b');
							possibleMoves.push_back(move + 'n');
						} else
							possibleMoves.push_back(createMove(piece->pos, pos3));
					}

				// En passant(left)
				if (canEnPassant(piece, 'l'))
					possibleMoves.push_back(createMove(piece->pos, piece->pos + (piece->color == WHITE ? Point<int>(-1, -1) : Point<int>(1, -1))));
			} else {
				if (cells[pos4.x][pos4.y] && cells[pos4.x][pos4.y]->color != piece->color)
					if (blockingCells[pos4.x][pos4.y]) {
						if (pos4.x == 0 || pos4.x == 7) {
							string move = createMove(piece->pos, pos4);

							possibleMoves.push_back(move + 'q');
							possibleMoves.push_back(move + 'r');
							possibleMoves.push_back(move + 'b');
							possibleMoves.push_back(move + 'n');
						} else
							possibleMoves.push_back(createMove(piece->pos, pos4));
					}

				// En passant(right)
				if (canEnPassant(piece, 'r'))
					possibleMoves.push_back(createMove(piece->pos, piece->pos + (piece->color == WHITE ? Point<int>(-1, 1) : Point<int>(1, 1))));
			}

			return;
		}

		// Column pin
		{
			Point<int> pos3, pos4;

			if (piece->color == WHITE) {
				pos3 = piece->pos + Point<int>(-1, 0);
				pos4 = piece->pos + Point<int>(-2, 0);
			} else {
				pos3 = piece->pos + Point<int>(1, 0);
				pos4 = piece->pos + Point<int>(2, 0);
			}

			if (!cells[pos3.x][pos3.y])
				if (blockingCells[pos3.x][pos3.y])
					possibleMoves.push_back(createMove(piece->pos, pos3));

			if (!piece->moves && !cells[pos3.x][pos3.y] && !cells[pos4.x][pos4.y])
				if (blockingCells[pos4.x][pos4.y])
					possibleMoves.push_back(createMove(piece->pos, pos4));
		}

		return;
	}

	Point<int> pos1, pos2, pos3, pos4;

	if (piece->color == WHITE) {
		pos1 = piece->pos + Point<int>(-1, -1);
		pos2 = piece->pos + Point<int>(-1, 0);
		pos3 = piece->pos + Point<int>(-1, 1);
		pos4 = piece->pos + Point<int>(-2, 0);
	} else {
		pos1 = piece->pos + Point<int>(1, -1);
		pos2 = piece->pos + Point<int>(1, 0);
		pos3 = piece->pos + Point<int>(1, 1);
		pos4 = piece->pos + Point<int>(2, 0);
	}

	if (isInside(pos1) && cells[pos1.x][pos1.y] && cells[pos1.x][pos1.y]->color != piece->color)
		if (blockingCells[pos1.x][pos1.y]) {
			if (pos1.x == 0 || pos1.x == 7) {
				string move = createMove(piece->pos, pos1);

				possibleMoves.push_back(move + 'q');
				possibleMoves.push_back(move + 'r');
				possibleMoves.push_back(move + 'b');
				possibleMoves.push_back(move + 'n');
			} else
				possibleMoves.push_back(createMove(piece->pos, pos1));
		}

	if (isInside(pos2) && !cells[pos2.x][pos2.y])
		if (blockingCells[pos2.x][pos2.y])
			if (pos2.x == 0 || pos2.x == 7) {
				string move = createMove(piece->pos, pos2);

				possibleMoves.push_back(move + 'q');
				possibleMoves.push_back(move + 'r');
				possibleMoves.push_back(move + 'b');
				possibleMoves.push_back(move + 'n');
			} else
				possibleMoves.push_back(createMove(piece->pos, pos2));

	if (isInside(pos3) && cells[pos3.x][pos3.y] && cells[pos3.x][pos3.y]->color != piece->color)
		if (blockingCells[pos3.x][pos3.y])
			if (pos3.x == 0 || pos3.x == 7) {
				string move = createMove(piece->pos, pos3);

				possibleMoves.push_back(move + 'q');
				possibleMoves.push_back(move + 'r');
				possibleMoves.push_back(move + 'b');
				possibleMoves.push_back(move + 'n');
			} else
				possibleMoves.push_back(createMove(piece->pos, pos3));

	if (!piece->moves && !cells[pos2.x][pos2.y] && !cells[pos4.x][pos4.y])
		if (blockingCells[pos4.x][pos4.y])
			possibleMoves.push_back(createMove(piece->pos, pos4));

	// En passant(left)
	if (canEnPassant(piece, 'l'))
		possibleMoves.push_back(createMove(piece->pos, piece->pos + (piece->color == WHITE ? Point<int>(-1, -1) : Point<int>(1, -1))));

	// En passant(right)
	if (canEnPassant(piece, 'r'))
		possibleMoves.push_back(createMove(piece->pos, piece->pos + (piece->color == WHITE ? Point<int>(-1, 1) : Point<int>(1, 1))));
}

Point<int> Board::stringToCoordinates(const string s) {
	return Point<int>(8 - (s[1] - '0'), s[0] - 'a');
}

string Board::coordinatesToString(Point<int> p) {
	return string(1, (char)('a' + p.y)) + to_string(8 - p.x);
}

string Board::createMove(Point<int> p1, Point<int> p2) {
	return coordinatesToString(p1) + coordinatesToString(p2);
}

void Board::showCurrentPieces(const char color, ostream& os) {
	if (color == WHITE)
		os << "Current white pieces:\n";
	else
		os << "Current black pieces:\n";

	for (vector<Piece*> v : currentPieces[color])
		for (Piece* p : v)
			os << pieceTypeToString(p) << ": " << coordinatesToString(p->pos) << '\n';
}

void Board::showRemovedPieces(ostream& os) {
	os << "Removed pieces:\n";

	for (pair<int, Piece*>& p : removedPieces)
		os << pieceTypeToString(p.second) << ": " << coordinatesToString(p.second->pos) << '\n';
}

void Board::showPinnedPieces(ostream& os) {
	if (color == WHITE)
		os << "White pinned pieces:\n";
	else
		os << "Black pinned pieces:\n";

	for (vector<Piece*> v : currentPieces[color])
		for (Piece* p : v)
			if (p->isPinned)
				os << pieceTypeToString(p) << ": " << coordinatesToString(p->pos) << '\n';
}

void Board::showUnpinnedPieces(ostream& os) {
	if (color == WHITE)
		os << "White pinned pieces:\n";
	else
		os << "Black pinned pieces:\n";

	for (vector<Piece*> v : currentPieces[color])
		for (Piece* p : v)
			if (!p->isPinned)
				os << pieceTypeToString(p) << ": " << coordinatesToString(p->pos) << '\n';
}

void Board::showPossibleMoves(ostream& os) {
	vector<string> possibleMoves;

	os << "All possible moves:\n";
	findPossibleMoves(possibleMoves);

	for (auto& move : possibleMoves)
		os << move << '\n';

	os << '\n' << possibleMoves.size();
}

void Board::advanceState(string moves) {
	int i = 0;

	while (i < moves.size()) {
		int start = i, end = i;

		while (end < moves.size() && moves[end] != ' ')
			end++;

		move(string(moves.begin() + start, moves.begin() + end));
		i = end + 1;
	}
}

void Board::move(pair<Point<int>, Point<int>> move, const char promotion) {
	Point<int> pos1 = move.first;
	Point<int> pos2 = move.second;

	Piece* piece1 = cells[pos1.x][pos1.y];

	if (piece1->type == PAWN && abs(pos1.y - pos2.y) == 1 && !cells[pos2.x][pos2.y]) {
		// En passant
		cells[pos1.x][pos1.y] = nullptr;
		cells[pos2.x][pos2.y] = piece1;

		removePiece(cells[pos1.x][pos2.y]);
		cells[pos1.x][pos2.y] = nullptr;
	} else if (piece1->type == KING && pos1.y == 4 && pos2.y == 2) {
		// Big castle
		Piece* piece2 = cells[pos1.x][0];

		piece2->pos = {pos2.x, 3};
		piece2->moves++;

		cells[pos1.x][pos1.y] = nullptr;
		cells[pos1.x][pos2.y] = piece1;

		cells[pos1.x][0] = nullptr;
		cells[pos1.x][3] = piece2;
	} else if (piece1->type == KING && pos1.y == 4 && pos2.y == 6) {
		// Small castle
		Piece* piece2 = cells[pos1.x][7];

		piece2->pos = {pos2.x, 5};
		piece2->moves++;

		cells[pos1.x][pos1.y] = nullptr;
		cells[pos1.x][pos2.y] = piece1;

		cells[pos1.x][7] = nullptr;
		cells[pos1.x][5] = piece2;
	} else {
		// Normal move
		Piece* piece2 = cells[pos2.x][pos2.y];

		cells[pos2.x][pos2.y] = piece1;
		cells[pos1.x][pos1.y] = nullptr;

		if (piece2)
			removePiece(piece2);

		if (pos2.x == 0 || pos2.x == 7)
			if (piece1->type == PAWN) {
				changeType(piece1, promotion);
				piece1->promotedAt = moveNo + 1;
			}
	}

	piece1->pos = pos2;
	piece1->moves++;

	prevMoves.push_back({{pos1, pos2}, getPositionKey()});
	color = (color == WHITE) ? BLACK : WHITE;
	moveNo++;
}

void Board::move(const string move) {
	Point<int> src = stringToCoordinates(string(move.begin(), move.begin() + 2));
	Point<int> dest = stringToCoordinates(string(move.begin() + 2, move.begin() + 4));

	if (move.size() >= 5) {
		if (move[4] == 'q')
			Board::move({src, dest}, QUEEN);
		else if (move[4] == 'r')
			Board::move({src, dest}, ROOK);
		else if (move[4] == 'b')
			Board::move({src, dest}, BISHOP);
		else
			Board::move({src, dest}, KNIGHT);
	} else
		Board::move({src, dest});
}

void Board::undo() {
	Point<int> pos1 = prevMoves.back().move.first;
	Point<int> pos2 = prevMoves.back().move.second;

	Piece* piece = cells[pos2.x][pos2.y];

	if (piece->type == PAWN && abs(pos1.y - pos2.y) == 1 && removedPieces.size() && removedPieces.back().first == moveNo &&
		removedPieces.back().second->pos.x == pos1.x && removedPieces.back().second->pos.y == pos2.y) {
		// En passant
		cells[pos2.x][pos2.y] = nullptr;
		cells[pos1.x][pos1.y] = piece;

		cells[pos1.x][pos2.y] = removedPieces.back().second;
		currentPieces[color][PAWN].push_back(removedPieces.back().second);

		removedPieces.pop_back();
	} else if (piece->type == KING && ((pos1 == Point<int>(0, 4) && pos2 == Point<int>(0, 2)) ||
									   (pos1 == Point<int>(7, 4) && pos2 == Point<int>(7, 2)))) {
		// Big castle
		cells[pos1.x][3]->pos = {pos1.x, 0};
		cells[pos1.x][3]->moves--;

		cells[pos1.x][2] = nullptr;
		cells[pos1.x][4] = piece;

		cells[pos1.x][0] = cells[pos2.x][3];
		cells[pos1.x][3] = nullptr;
	} else if (piece->type == KING && ((pos1 == Point<int>(0, 4) && pos2 == Point<int>(0, 6)) ||
		                               (pos1 == Point<int>(7, 4) && pos2 == Point<int>(7, 6)))) {
		// Small castle
		cells[pos1.x][5]->pos = {pos1.x, 7};
		cells[pos1.x][5]->moves--;

		cells[pos1.x][6] = nullptr;
		cells[pos1.x][4] = piece;

		cells[pos1.x][7] = cells[pos2.x][5];
		cells[pos1.x][5] = nullptr;
	} else {
		// Normal move
		if (removedPieces.size() && removedPieces.back().first == moveNo) {
			cells[pos2.x][pos2.y] = removedPieces.back().second;
			currentPieces[color][removedPieces.back().second->type].push_back(removedPieces.back().second);
			removedPieces.pop_back();
		} else
			cells[pos2.x][pos2.y] = nullptr;

		cells[pos1.x][pos1.y] = piece;

		if (piece->promotedAt == moveNo) {
			changeType(piece, PAWN);
			piece->promotedAt = 0;
		}
	}

	piece->pos = pos1;
	piece->moves--;
	
	prevMoves.pop_back();
	color = (color == WHITE) ? BLACK : WHITE;
	moveNo--;
}

void Board::reset() {
	while (prevMoves.size())
		undo();
}

ostream& operator<<(ostream& os, Board& board) {
	for (int i = 0; i < 8; i++) {
		os << 8 - i << "| ";

		for (int j = 0; j < 8; j++)
			if (!board.cells[i][j])
				os << "-  ";
			else
				os << board.pieceTypeToString(board.cells[i][j]) << ' ';

		os << '\n';
	}

	os << "   -----------------------\n   ";

	for (int j = 0; j < 8; j++)
		os << (char)('a' + j) << "  ";

	return os;
}
