#pragma once

#include "chess.hpp"
#include <unordered_map>

using namespace chess;

const int PAWN_VALUE = 1;
const int KNIGHT_VALUE = 3;
const int BISHOP_VALUE = 3;
const int ROOK_VALUE = 5;
const int QUEEN_VALUE = 9;
const int KING_VALUE = 1000;
const int QUIESCENCE_SEARCH_DEPTH = 50;

struct TTEntry {
	int eval;
	int depth;
};

bool compare_moves(Move a, Move b);
std::string find_best_move(const std::string& fen, int depth);
void order_moves(Board board, Movelist& moves);
int score_move(Board board, Move move);
int quiescence_search(Board& board, std::unordered_map<uint64_t, TTEntry>& transpositions, int alpha, int beta);
int search(Board& board, std::unordered_map<uint64_t, TTEntry>& transpositions, int depth, int ply, int alpha, int beta);
PieceType get_piece_type(Board board, Square square);
int evaluate(Board board);
int count_material(Board board, Color side);
int get_piece_value(PieceType type);
