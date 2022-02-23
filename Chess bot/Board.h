#pragma once

#include "utils.h"
#include "Piece.h"

class Board {
private:
	struct MoveHistory {
		pair<Point<int>, Point<int>> move;
		string posKey;
	};

	void removePiece(Piece* piece);

	void changeType(Piece* piece, const char type);

	string pieceTypeToString(Piece* piece) const;

	void markPinnedPieces();

	void unmarkPinnedPieces();

	void markBlockingCells();

	void markBlockingCells(Point<int> checkPos);

	void unmarkBlockingCells();

	void markCellsGuardedByRook(Piece* piece);

	void markCellsGuardedByKnight(Piece* piece);

	void markCellsGuardedByBishop(Piece* piece);

	void markCellsGuardedByQueen(Piece* piece);

	void markCellsGuardedByKing(Piece* piece);

	void markCellsGuardedByPawn(Piece* piece);

	void setPosFactorsForRook(Piece* piece);

	void setPosFactorsForKnight(Piece* piece);

	void setPosFactorsForBishop(Piece* piece);

	void setPosFactorsForQueen(Piece* piece);

	void setPosFactorsForKing(Piece* piece);

	void setPosFactorsForPawn(Piece* piece);

	bool canEnPassant(Piece* const piece, const char dir) const;

public:
	int halfmoveClock = 0, moveNo = 0;
	char color = WHITE;

	bool guardedCells[8][8], blockingCells[8][8];

	vector<vector<vector<Piece*>>> currentPieces;
	vector<pair<int, Piece*>> removedPieces;

	vector<vector<Piece*>> cells;
	vector<MoveHistory> prevMoves;

	Board();

	Board(int);

	Board(const string fen);

	~Board();

	string getFen();

	string getPositionKey();

	void setPosFactors(Piece* piece);

	bool isIncheck();

	bool isInside(Point<int> pos) const;

	void guardedBy(vector<Point<int>>& guards, Point<int> pos, const char color, bool all = false);

	void markGuardedCells(const char color);

	void unmarkGuardedCells();

	void findPossibleMoves(vector<string>& possibleMoves);

	void findPossibleMovesForRook(Piece* piece, vector<string>& possibleMoves);

	void findPossibleMovesForKnight(Piece* piece, vector<string>& possibleMoves);

	void findPossibleMovesForBishop(Piece* piece, vector<string>& possibleMoves);

	void findPossibleMovesForQueen(Piece* piece, vector<string>& possibleMoves);

	void findPossibleMovesForKing(Piece* piece, vector<string>& possibleMoves, bool inCheck = false);

	void findPossibleMovesForPawn(Piece* piece, vector<string>& possibleMoves);

	static Point<int> stringToCoordinates(const string s);

	static string coordinatesToString(Point<int> p);

	static string createMove(Point<int> p1, Point<int> p2);

	void showCurrentPieces(const char color, ostream& os);

	void showRemovedPieces(ostream& os);

	void showPinnedPieces(ostream& os);

	void showUnpinnedPieces(ostream& os);

	void showPossibleMoves(ostream& os);

	void advanceState(const string moves);

	void move(const pair<Point<int>, Point<int>> move, const char promotion = NONE);

	void move(const string move);

	void undo();

	void reset();

	friend ostream& operator<<(ostream& os, Board& board);
};
