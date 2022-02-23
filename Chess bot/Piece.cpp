#include "Piece.h"

Piece::Piece(int x, int y, char type, char color, char score) : pos(x, y) {
    this->type = type; this->color = color; this->score = score;
}

Piece::Piece(Point<int> point, char type, char color, char score) : pos(point.x, point.y) {
    this->type = type; this->color = color; this->score = score;;
}
