#pragma once

#include "chess.hpp"

using namespace chess;

const int PAWN_VALUE = 1;
const int KNIGHT_VALUE = 3;
const int BISHOP_VALUE = 3;
const int ROOK_VALUE = 5;
const int QUEEN_VALUE = 9;
const int Infinity = 1000000;

int GetPieceValue(PieceType type);
int CountMaterial(const Board& board, Color side);
int Evaluate(const Board& board);
PieceType GetPieceType(const Board& board, Square square);
int Search(Board board, int depth, int alpha, int beta);
Move FindBestMove(Board board, int searchDepth);
