#include "bot.hpp"
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

int numPositionsEvaluated = 0;

int getNumPositionsEvaluated() {
	return numPositionsEvaluated;
}

int getPieceValue(PieceType type) {
	if (type == PieceType::PAWN) {
		return PAWN_VALUE;
	}
	else if (type == PieceType::BISHOP) {
		return BISHOP_VALUE;
	}
	else if (type == PieceType::ROOK) {
		return ROOK_VALUE;
	}
	else if (type == PieceType::QUEEN) {
		return QUEEN_VALUE;
	}
	else if (type == PieceType::KNIGHT) {
		return KNIGHT_VALUE;
	}
	else if (type == PieceType::KING) {
		return KING_VALUE;
	}
	return 0;
}

int countMaterial(const Board& board, Color side) {
	Bitboard us = board.us(side);
	int pawns = (board.pieces(PieceType::PAWN) & us).count() * PAWN_VALUE;
	int knights = (board.pieces(PieceType::KNIGHT) & us).count() * KNIGHT_VALUE;
	int bishops = (board.pieces(PieceType::BISHOP) & us).count() * BISHOP_VALUE;
	int rooks = (board.pieces(PieceType::ROOK) & us).count() * ROOK_VALUE;
	int queens = (board.pieces(PieceType::QUEEN) & us).count() * QUEEN_VALUE;
	return pawns + knights + bishops + rooks + queens;
}

int evaluate(const Board& board) {
	numPositionsEvaluated++;
	int whiteEval = countMaterial(board, Color::WHITE);
	int blackEval = countMaterial(board, Color::BLACK);

	int evaluation = whiteEval - blackEval;

	if (board.sideToMove() == Color::WHITE) {
		return evaluation;
	}
	else {
		return -evaluation;
	}
}

bool compareMoves(Move a, Move b) {
	return a.score() > b.score();
}

PieceType getPieceType(const Board& board, Square square) {
	Bitboard squareBit;
	squareBit = squareBit |= (1ULL << square.index());
	uint64_t pawn = (squareBit & board.pieces(PieceType::PAWN)).getBits();
	if (pawn) return PieceType::PAWN;

	uint64_t bishop = (squareBit & board.pieces(PieceType::BISHOP)).getBits();
	if (bishop) return PieceType::BISHOP;

	uint64_t knight = (squareBit & board.pieces(PieceType::KNIGHT)).getBits();
	if (knight) return PieceType::KNIGHT;

	uint64_t rook = (squareBit & board.pieces(PieceType::ROOK)).getBits();
	if (rook) return PieceType::ROOK;

	uint64_t queen = (squareBit & board.pieces(PieceType::QUEEN)).getBits();
	if (queen) return PieceType::QUEEN;

	uint64_t king = (squareBit & board.pieces(PieceType::KING)).getBits();
	if (king) return PieceType::KING;

	return PieceType::NONE;
}

int oldSearch(Board board, int depth) {
	if (depth == 0) return evaluate(board);
	Movelist moves;
	movegen::legalmoves(moves, board);
	if(moves.size() == 0) {
		if(board.inCheck()) {
			return -KING_VALUE;
		}
		return 0;
	}
	int bestEval = -KING_VALUE;
	for(auto& move: moves) {
		board.makeMove(move);
		int eval = -oldSearch(board, depth - 1);
		bestEval = std::max(eval, bestEval);
		board.unmakeMove(move);
	}
	return bestEval;
}

int search(Board board, int depth, int alpha, int beta) {
	if (depth == 0) {
		return evaluate(board);
	}

	Movelist moves;
	movegen::legalmoves(moves, board);
	if (moves.size() == 0) {
		if (board.inCheck()) {
			return -KING_VALUE;
		}
		return 0;
	}

	for (const auto& move : moves) {
		board.makeMove(move);
		int eval = -search(board, depth - 1, -beta, -alpha);
		board.unmakeMove(move);
		if (eval >= beta) {
			return beta;
		}
		alpha = std::max(alpha, eval);
	}

	return alpha;
}

Move findBestMove(Board board, int searchDepth) {
	Movelist moves;
	movegen::legalmoves(moves, board);

	int bestMoveEval = -KING_VALUE;
	int bestMoveIndex = 0;
	for (int i = 0; i < moves.size(); i++) {
		board.makeMove(moves[i]);
		int evaluation = -search(board, searchDepth, -KING_VALUE, KING_VALUE);
		board.unmakeMove(moves[i]);
		if(evaluation > bestMoveEval) {
			bestMoveEval = evaluation;
			bestMoveIndex = i;
		}
	}

	return moves[bestMoveIndex];
}