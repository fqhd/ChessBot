#pragma once

#include "chess.hpp"

using namespace chess;

int getPieceValue(PieceType type);
int countMaterial(const Board& board, Color side);
int evaluate(const Board& board);
bool compareMoves(Move a, Move b);
PieceType getPieceType(const Board& board, Square square);
int calculateMoveScore(const Board& board, Move move);
int search(Board board, int depth, int alpha, int beta);
void worker(const Board& board, int searchDepth, int* result);
Move findBestMove(Board board, int searchDepth);
