#include "Board.h"
#include "AI.h"

int main() {
    Board* board = new Board;

    bool editMode = false;
    char currColor = WHITE;

    char buffer[256];
    int depth = 4;

    setvbuf(stdin, NULL, _IOLBF, BUFSIZ);
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

    while (true) {
        fflush(stdout);
        fgets(buffer, 256, stdin);

        if (strstr(buffer, "xboard"))
            continue;

        if (strstr(buffer, "protover 2")) {
            cout << "feature sigint=0 sigterm=0 san=0 name=Maximuss\n";
            continue;
        }

        if (strstr(buffer, "new")) {
            if (board)
                delete board;

            board = new Board;
            editMode = false;
            continue;
        }

        if (strstr(buffer, "white")) {
            currColor = WHITE;
            continue;
        }

        if (strstr(buffer, "black")) {
            currColor = BLACK;
            continue;
        }

        if (strstr(buffer, "edit")) {
            delete board;
            editMode = true;
            currColor = WHITE;
            board = new Board(true);
            continue;
        }

        if (strstr(buffer, "force")) {
            editMode = true;
            continue;
        }

        if (editMode)
            if (buffer[0] == 'R' || buffer[0] == 'N' || buffer[0] == 'B' || buffer[0] == 'Q' || buffer[0] == 'K' || buffer[0] == 'P')
                if (buffer[1] >= 'a' && buffer[1] <= 'h' && buffer[2] >= '1' && buffer[2] <= '8') {
                    Piece* piece = new Piece(Board::stringToCoordinates({buffer + 1, 2}), NONE, currColor, NONE);

                    if (buffer[0] == 'R') {
                        piece->type = ROOK;
                        piece->score = 5;
                    } else if (buffer[0] == 'N') {
                        piece->type = KNIGHT;
                        piece->score = 3;
                    } else if (buffer[0] == 'B') {
                        piece->type = BISHOP;
                        piece->score = 3;
                    } else if (buffer[0] == 'Q') {
                        piece->type = QUEEN;
                        piece->score = 9;
                    } else if (buffer[0] == 'K') {
                        piece->type = KING;
                        piece->score = 0;
                    } else {
                        piece->type = PAWN;
                        piece->score = 1;
                    }

                    board->setPosFactors(piece);
                    board->cells[piece->pos.x][piece->pos.y] = piece;
                    board->currentPieces[currColor][piece->type].push_back(piece);
                    continue;
                }

        if (editMode)
            if (buffer[0] == 'c' && !(buffer[1] >= '1' && buffer[1] <= '8')) {
                currColor = BLACK;
                continue;
            }

        if (editMode)
            if (buffer[0] == '.') {
                editMode = true;
                continue;
            }

        if (strstr(buffer, "go")) {
            string bestMove = "";
            string fen = board->getFen();

            delete board;
            board = new Board(fen);

            editMode = false;
            board->color = currColor;

            adjustDepth(board, depth);
            search(bestMove, board, 0, depth , board->color, -INF, INF, false);

            if (bestMove != "") {
                cout << "move " << bestMove << '\n';
                board->move(bestMove);
                adjustDepth(board, depth);
            }

            continue;
        }

        if (strstr(buffer, "quit"))
            break;

        if (buffer[0] >= 'a' && buffer[0] <= 'h' && buffer[1] >= '1' && buffer[1] <= '8') {
            board->move(buffer);

            if (!editMode) {
                string bestMove = "";

                adjustDepth(board, depth);
                search(bestMove, board, 0, depth, board->color, -INF, INF, false);

                if (bestMove != "") {
                    cout << "move " << bestMove << '\n';
                    board->move(bestMove);
                }
            }
        }
    }

    delete board;
    return 0;
}
