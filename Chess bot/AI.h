#pragma once

#include "utils.h"
#include "Board.h"

GAME_TYPE getGameType(Board* board) {
	int whiteRooks   = 0, blackRooks   = 0;
	int whiteKnights = 0, blackKnights = 0;
	int whiteBishops = 0, blackBishops = 0;
	
	int whiteQueens  = 0, blackQueens  = 0;
	int whitePawns   = 0, blackPawns   = 0;
	int whitePieces  = 0, blackPieces  = 0;

	for (vector<Piece*> v : board->currentPieces[WHITE])
		for (Piece* p : v) {
			if (p->type == ROOK)
				whiteRooks++;
			else if (p->type == KNIGHT)
				whiteKnights++;
			else if (p->type == BISHOP)
				whiteBishops++;
			else if (p->type == QUEEN)
				whiteQueens++;
			else if (p->type == PAWN)
				whitePawns++;

			whitePieces++;
		}

	for (vector<Piece*> v : board->currentPieces[BLACK])
		for (Piece* p : v) {
			if (p->type == ROOK)
				blackRooks++;
			else if (p->type == KNIGHT)
				blackKnights++;
			else if (p->type == BISHOP)
				blackBishops++;
			else if (p->type == QUEEN)
				blackQueens++;
			else if (p->type == PAWN)
				blackPawns++;

			blackPieces++;
		}

	if (whitePawns + blackPawns == whitePieces + blackPieces - 2)
		return KINGS_AND_PAWNS_ENDGAME;

	if (whitePieces + blackPieces == 3 && (whiteQueens || blackQueens))
		return KINGS_AND_ONE_QUEEN_ENDGAME;

	return MIDDLEGAME;
}

void adjustDepth(Board* board, int& depth) {
	GAME_TYPE gameType = getGameType(board);

	if (gameType == KINGS_AND_ONE_QUEEN_ENDGAME)
		depth = 8;
}

double pawnStructureEvaluation(Board* board, const char color) {
	double score = 0;
	int dir = color == WHITE ? -1 : 1;
	bool pawnsPos[8][8] {};

	for (Piece* p : board->currentPieces[color][PAWN]) {
		score += p->score + p->posFactors[p->pos.x][p->pos.y];
		pawnsPos[p->pos.x][p->pos.y] = true;
	}

	// Check for "doubled" pawns
	for (int j = 0; j < 8; j++) {
		int noOfPawns = 0;

		for (int i = 0; i < 8; i++)
			if (board->cells[i][j] && board->cells[i][j]->type == PAWN)
				noOfPawns++;

		if (noOfPawns > 1)
			score -= noOfPawns * PAWN_SCORE / 2;
	}

	// Check for passed pawns
	board->markGuardedCells(color == WHITE ? BLACK : WHITE);

	for (Piece* p : board->currentPieces[color][PAWN]) {
		int i;
		bool stop = false;

		// Check for an empty file
		for (i = p->pos.x + dir; i != 0 && i != 7; i += dir)
			if (board->cells[i][p->pos.y] || board->guardedCells[i][p->pos.y]) {
				stop = true;
				break;
			}

		if (!stop)
			score += 2 / abs(i - p->pos.x);
	}

	board->unmarkGuardedCells();
	return score;
}

double middlegameEvaluation(Board* board, const char color, int depth) {
	double score1 = 0, score2 = 0;

	for (vector<Piece*> v : board->currentPieces[WHITE])
		for (Piece* p : v)
			if (p->type != PAWN)
				score1 += p->score + p->posFactors[p->pos.x][p->pos.y];

	for (vector<Piece*> v : board->currentPieces[BLACK])
		for (Piece* p : v)
			if (p->type != PAWN)
				score2 += p->score + p->posFactors[p->pos.x][p->pos.y];

	return (color == WHITE) ? (score1 - score2) : (score2 - score1);
}

double kingsAndPawnsEndgameEvaluation(Board* board, const char color) {
	double score1 = 0, score2 = 0;

	double posFactorsForKing[8][8]{
		{-0.50, -0.35, -0.35, -0.35, -0.35, -0.35, -0.35, -0.50},
		{-0.35,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00, -0.35},
		{-0.35,  0.00,  0.15,  0.15,  0.15,  0.15,  0.00, -0.35},
		{-0.35,  0.00,  0.20,  0.20,  0.20,  0.20,  0.00, -0.35},
		{-0.35,  0.00,  0.20,  0.20,  0.20,  0.20,  0.00, -0.35},
		{-0.35,  0.00,  0.15,  0.15,  0.15,  0.15,  0.00, -0.35},
		{-0.35,  0.00,  0.00,  0.00,  0.00,  0.00,  0.00, -0.35},
		{-0.50, -0.35, -0.35, -0.35, -0.35, -0.35, -0.35, -0.50}
	};

	double posFactorsForPawn[8][8]{
		{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
		{0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50},
		{0.40, 0.40, 0.40, 0.40, 0.40, 0.40, 0.40, 0.40},
		{0.30, 0.30, 0.30, 0.30, 0.30, 0.30, 0.30, 0.30},
		{0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20, 0.20},
		{0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10, 0.10},
		{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00},
		{0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00}
	};

	Piece* whiteKing = board->currentPieces[WHITE][KING][0];
	Piece* blackKing = board->currentPieces[BLACK][KING][0];

	score1 += posFactorsForKing[whiteKing->pos.x][whiteKing->pos.y];
	score2 += posFactorsForKing[blackKing->pos.x][blackKing->pos.y];

	for (Piece* p : board->currentPieces[WHITE][PAWN]) {
		if (Point<int>::getEuclidianDistance(whiteKing->pos, p->pos) < 2)
			score1 += posFactorsForPawn[p->pos.x][p->pos.y] * 2;

		score1 += p->score + posFactorsForPawn[p->pos.x][p->pos.y];
	}

	for (Piece* p : board->currentPieces[BLACK][PAWN]) {
		if (Point<int>::getEuclidianDistance(blackKing->pos, p->pos) < 2)
			score2 += posFactorsForPawn[7 - p->pos.x][7 - p->pos.y] * 2;

		score2 += p->score + posFactorsForPawn[7 - p->pos.x][7 - p->pos.y];
	}

	return (color == WHITE) ? (score1 - score2) : (score2 - score1);
}

double kingsAndOneQueenEndgameEvaluation(Board* board, const char color) {
	double score1 = 0, score2 = 0;

	double posFactorsForKing[8][8]{
		{-0.50, -0.35, -0.35, -0.35, -0.35, -0.35, -0.35, -0.50},
		{-0.35,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10, -0.35},
		{-0.35,  0.10,  0.15,  0.15,  0.15,  0.15,  0.10, -0.35},
		{-0.35,  0.10,  0.25,  0.25,  0.25,  0.25,  0.10, -0.35},
		{-0.35,  0.10,  0.25,  0.25,  0.25,  0.25,  0.10, -0.35},
		{-0.35,  0.10,  0.15,  0.15,  0.15,  0.15,  0.10, -0.35},
		{-0.35,  0.10,  0.10,  0.10,  0.10,  0.10,  0.10, -0.35},
		{-0.50, -0.35, -0.35, -0.35, -0.35, -0.35, -0.35, -0.50}
	};

	Piece* whiteKing = board->currentPieces[WHITE][KING][0];
	Piece* blackKing = board->currentPieces[BLACK][KING][0];

	score1 += posFactorsForKing[whiteKing->pos.x][whiteKing->pos.y];
	score2 += posFactorsForKing[blackKing->pos.x][blackKing->pos.y];

	if (board->currentPieces[WHITE][QUEEN].size()) {
		Piece* whiteQueen = board->currentPieces[WHITE][QUEEN][0];

		if (blackKing->pos.x < 4 && whiteQueen->pos.x == blackKing->pos.x + 1)
			score1 += 5 - blackKing->pos.x;
		else if (blackKing->pos.x >= 4 && whiteQueen->pos.x == blackKing->pos.x - 1)
			score1 += blackKing->pos.x - 4;
		else if (blackKing->pos.y < 4 && whiteQueen->pos.y == blackKing->pos.y + 1)
			score1 += 5 - blackKing->pos.y;
		else if (blackKing->pos.y >= 4 && whiteQueen->pos.y == blackKing->pos.y - 1)
			score1 += blackKing->pos.y - 4;

		score1 += whiteQueen->score;

		if (Point<int>::getEuclidianDistance(whiteQueen->pos, blackKing->pos) < 2) {
			if (Point<int>::getEuclidianDistance(whiteQueen->pos, whiteKing->pos) < 2)
				score1 += 0.5;
			else
				score1 -= whiteQueen->score;
		}

		score1 += 1 / Point<int>::getEuclidianDistance(whiteKing->pos, blackKing->pos);

		if (board->color == BLACK) {
			vector<string> possibleMoves;

			if (board->isIncheck())
				score1 += 0.5;

			board->findPossibleMoves(possibleMoves);
			score2 += possibleMoves.size() / 5;
		}
	} else {
		Piece* blackQueen = board->currentPieces[BLACK][QUEEN][0];

		if (whiteKing->pos.x < 4 && blackQueen->pos.x == whiteKing->pos.x + 1)
			score2 += 5 - whiteKing->pos.x;
		else if (whiteKing->pos.x >= 4 && blackQueen->pos.x == whiteKing->pos.x - 1)
			score2 += whiteKing->pos.x - 4;
		else if (whiteKing->pos.y < 4 && blackQueen->pos.y == whiteKing->pos.y + 1)
			score2 += 5 - whiteKing->pos.y;
		else if (whiteKing->pos.y >= 4 && blackQueen->pos.y == whiteKing->pos.y - 1)
			score2 += whiteKing->pos.y - 4;

		score2 += blackQueen->score;

		if (Point<int>::getEuclidianDistance(blackQueen->pos, whiteKing->pos) < 2) {
			if (Point<int>::getEuclidianDistance(blackQueen->pos, blackKing->pos) < 2)
				score2 += 0.5;
			else
				score2 -= blackQueen->score;
		}

		score2 += 1 / Point<int>::getEuclidianDistance(blackKing->pos, whiteKing->pos);

		if (board->color == WHITE) {
			vector<string> possibleMoves;

			if (board->isIncheck())
				score2 += 0.5;

			board->findPossibleMoves(possibleMoves);
			score1 += possibleMoves.size() / 5;
		}
	}

	return (color == WHITE) ? (score1 - score2) : (score2 - score1);
}

double evaluate(Board* board, const char color, int depth) {
	GAME_TYPE gameType = getGameType(board);

	if (gameType == KINGS_AND_PAWNS_ENDGAME)
		return kingsAndPawnsEndgameEvaluation(board, color);
	else if (gameType == KINGS_AND_ONE_QUEEN_ENDGAME)
		return kingsAndOneQueenEndgameEvaluation(board, color);

	return middlegameEvaluation(board, color, depth);
}

bool shouldExtraSearch(Board* board) {
	Piece* piece = board->cells[board->prevMoves.back().move.second.x][board->prevMoves.back().move.second.y];

	if (piece->score >= 5) {
		vector<Point<int>> guardPos;

		board->guardedBy(guardPos, board->prevMoves.back().move.second, board->color == WHITE ? BLACK : WHITE);

		if (guardPos.size())
			return true;
	}

	return false;
}

double search(string& bestMove, Board* board, int currDepth, int maxDepth, const char color, double alpha, double beta, bool blockedDepth) {
	// Three fold repetition
	if (board->prevMoves.size()) {
		int no = 0;

		for (int i = 0; i < board->prevMoves.size() - 1; i++) {
			if (board->prevMoves[i].posKey == board->prevMoves.back().posKey)
				no++;

			if (no == 2)
				return 0;
		}
	}

	vector<string> possibleMoves;

	board->findPossibleMoves(possibleMoves);

	// Checkmate or stalemate
	if (!possibleMoves.size()) {
		if (!board->isIncheck())
			return 0;

		if (currDepth % 2 == 0)
			return -INF;

		return INF;
	}

	if (currDepth == maxDepth) {
		double score = evaluate(board, color, currDepth);

		if (!blockedDepth && shouldExtraSearch(board)) {
			if (board->color == color)
				maxDepth++;
			else
				maxDepth += 2;

			blockedDepth = true;
		} else
			return score;
	}

	vector<pair<string, double>> movesScores(possibleMoves.size());

	for (int i = 0; i < movesScores.size(); i++) {
		double score;

		board->move(possibleMoves[i]);
		score = evaluate(board, color, currDepth + 1);
		board->undo();

		movesScores[i].first = possibleMoves[i];
		movesScores[i].second = score;
	}

	double bestScore, currScore;

	if (currDepth % 2 == 0) {
		// Maximizing player
		bestScore = -INF;

		sort(movesScores.begin(), movesScores.end(), [](const auto& p1, const auto& p2) {
			return p1.second > p2.second;
		});

		for (auto& p : movesScores) {
			board->move(p.first);
			currScore = search(bestMove, board, currDepth + 1, maxDepth, color, alpha, beta, blockedDepth);

			if (currScore > bestScore) {
				bestMove = !currDepth ? p.first : bestMove;
				bestScore = currScore;
			} else if (!currDepth && bestMove == "")
				bestMove = p.first;

			board->undo();
			alpha = max(alpha, bestScore);

			if (alpha >= beta)
				return bestScore;
		}

		return bestScore;
	}

	// Minimizing player
	bestScore = INF;

	sort(movesScores.begin(), movesScores.end(), [](const auto& p1, const auto& p2) {
		return p1.second < p2.second;
	});

	for (auto& p : movesScores) {
		board->move(p.first);
		bestScore = min(bestScore, search(bestMove, board, currDepth + 1, maxDepth, color, alpha, beta, blockedDepth));
		board->undo();
		beta = min(beta, bestScore);

		if (beta <= alpha)
			return bestScore;
	}

	return bestScore;
}

// For debugging
/*struct TreeNode {
	double score;
	string move;
	vector<TreeNode*> children;
};*/

/*double calculateAllPaths(TreeNode* root, Board* board, int currDepth, int maxDepth, const char color) {
	if (currDepth == maxDepth)
		return evaluate(board, currDepth, color);

	vector<string> possibleMoves;
	double bestScore;

	board->findPossibleMoves(possibleMoves);

	// Checkmate or stalemate
	if (!possibleMoves.size()) {
		if (!board->isIncheck())
			return 0;

		if (currDepth % 2 == 0)
			return -INF;

		return INF;
	}

	if (currDepth == maxDepth)
		return evaluate(board, color);

	if (currDepth % 2 == 0) {
		// Maximizing player
		bestScore = -INF;

		for (auto& move : possibleMoves) {
			TreeNode* newNode = new TreeNode;

			newNode->move = move;
			root->children.push_back(newNode);

			board->move(move);
			newNode->score = calculateAllPaths(newNode, board, currDepth + 1, maxDepth, color);
			bestScore = max(bestScore, newNode->score);
			board->undo();
		}
	} else {
		// Minimizing player
		bestScore = INF;

		for (auto& move : possibleMoves) {
			TreeNode* newNode = new TreeNode;

			newNode->move = move;
			root->children.push_back(newNode);

			board->move(move);
			newNode->score = calculateAllPaths(newNode, board, currDepth + 1, maxDepth, color);
			bestScore = min(bestScore, newNode->score);
			board->undo();
		}
	}

	return bestScore;
}*/

/*void printBestPath(TreeNode* root, int depth) {
	if (!root)
		return;

	if (!root->children.size())
		return;

	TreeNode* nextNode = nullptr;
	double bestScore;

	if (depth % 2 == 0) {
		bestScore = -INF;

		for (int i = 0; i < root->children.size(); i++)
			if (root->children[i]->score > bestScore) {
				bestScore = root->children[i]->score;
				nextNode = root->children[i];
			}
	} else {
		bestScore = INF;

		for (int i = 0; i < root->children.size(); i++)
			if (root->children[i]->score < bestScore) {
				bestScore = root->children[i]->score;
				nextNode = root->children[i];
			}
	}

	if (nextNode)
		cout << nextNode->move << ' ';

	printBestPath(nextNode, depth + 1);
}

int noOfStates(Board* board, int depth) {
	if (!depth)
		return 1;

	int no = 0;
	vector<string> possibleMoves;

	board->findPossibleMoves(possibleMoves);

	for (auto& move : possibleMoves) {
		board->move(move);
		no += noOfStates(board, depth - 1);

		if (depth == 2)
			cout << move << ": " << noOfStates(board, depth - 1) << '\n';

		board->undo();
	}

	return no;
}*/
