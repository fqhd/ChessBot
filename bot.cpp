#include "bot.hpp"
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

int GetPieceValue(PieceType type) {
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
		return Infinity;
	}
	return 0;
}

int CountMaterial(const Board& board, Color side) {
	Bitboard us = board.us(side);
	int pawns = (board.pieces(PieceType::PAWN) & us).count() * PAWN_VALUE;
	int knights = (board.pieces(PieceType::KNIGHT) & us).count() * KNIGHT_VALUE;
	int bishops = (board.pieces(PieceType::BISHOP) & us).count() * BISHOP_VALUE;
	int rooks = (board.pieces(PieceType::ROOK) & us).count() * ROOK_VALUE;
	int queens = (board.pieces(PieceType::QUEEN) & us).count() * QUEEN_VALUE;
	return pawns + knights + bishops + rooks + queens;
}

int Evaluate(const Board& board) {
	int whiteEval = CountMaterial(board, Color::WHITE);
	int blackEval = CountMaterial(board, Color::BLACK);

	int evaluation = whiteEval - blackEval;

	if (board.sideToMove() == Color::WHITE) {
		return evaluation;
	}
	else {
		return -evaluation;
	}
}

PieceType GetPieceType(const Board& board, Square square) {
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

int Search(Board board, int depth) {
	if(depth == 0) return Evaluate(board);

	Movelist moves;
	movegen::legalmoves(moves, board);
	if(moves.size() == 0) {
		if(board.inCheck()) {
			return -Infinity;
		}
		return 0;
	}

	int bestEvaluation = -Infinity;
	for(auto& move : moves) {
		board.makeMove(move);
		int evaluation = -Search(board, depth-1);
		bestEvaluation = std::max(evaluation, bestEvaluation);
		board.unmakeMove(move);
	}

	return bestEvaluation;
}

Move FindBestMove(Board board, int searchDepth) {
	Movelist moves;
	movegen::legalmoves(moves, board);
	int bestMoveEvaluation = -Infinity;
	int bestMoveIndex = 0;
	for(unsigned int i = 0; i < moves.size(); i++) {
		board.makeMove(moves[i]);
		int evaluation = -Search(board, searchDepth-1);
		if (evaluation > bestMoveEvaluation) {
			bestMoveEvaluation = evaluation;
			bestMoveIndex = i;
		}
		board.unmakeMove(moves[i]);
	}
	return moves[bestMoveIndex];
}