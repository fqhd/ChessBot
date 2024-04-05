#include "bot.hpp"
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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