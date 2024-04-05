#include "bot.hpp"
#include <thread>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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

int kingEndgameEval(Square friendlyKing, Square opponentKing, float endgameWeight) {
	// 4K3/4P3/3q4/8/8/8/6k1/8 b - - 0 1 // black should move the king here
	int evaluation = 0;

	int dstBetweenKings = Square::distance(opponentKing, friendlyKing);
	evaluation -= dstBetweenKings;

	return (int)(evaluation * endgameWeight);
}

int evaluate(const Board& board) {
	int whiteEval = countMaterial(board, Color::WHITE);
	int blackEval = countMaterial(board, Color::BLACK);

	int evaluation = whiteEval - blackEval;

	float endGameEval = evaluation / 10.0f;
	
	evaluation += kingEndgameEval(board.kingSq(Color::WHITE), board.kingSq(Color::BLACK), endGameEval);

	if(board.sideToMove() == Color::WHITE) {
		return evaluation;
	} else {
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

int calculateMoveScore(const Board& board, Move move) {
	int moveScoreGuess = 0;
	PieceType fromType = getPieceType(board, move.from());
	PieceType toType = getPieceType(board, move.to());

	if (toType != PieceType::NONE) {
		moveScoreGuess = 10 * getPieceValue(toType) - getPieceValue(fromType);
	}

	if (move == Move::PROMOTION) {
		moveScoreGuess += getPieceValue(move.promotionType());
	}

	if (board.isAttacked(move.to(), ~board.sideToMove())) {
		moveScoreGuess -= getPieceValue(fromType);
	}

	return moveScoreGuess;
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
	for (auto& move : moves) {
		move.setScore(calculateMoveScore(board, move));
	}
	std::sort(moves.begin(), moves.end(), compareMoves);

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

void worker(const Board& board, int searchDepth, int* result) {
	int evaluation = -search(board, searchDepth, -KING_VALUE, KING_VALUE);
	*result = evaluation;
}

Move findBestMove(Board board, int searchDepth) {
	Movelist moves;
	movegen::legalmoves(moves, board);
	for (auto& move : moves) {
		move.setScore(calculateMoveScore(board, move));
	}
	std::sort(moves.begin(), moves.end(), compareMoves);

	std::vector<std::thread> threads;
	std::vector<int> evaluations(moves.size());

	for (int i = 0; i < moves.size(); i++) {
		board.makeMove(moves[i]);
		threads.push_back(std::thread(worker, board, searchDepth, &evaluations[i]));
		board.unmakeMove(moves[i]);
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}

	int bestMoveEval = -KING_VALUE;
	int bestMoveIndex = 0;
	for (int i = 0; i < evaluations.size(); i++) {
		if (evaluations[i] > bestMoveEval) {
			bestMoveEval = evaluations[i];
			bestMoveIndex = i;
		}
	}

	return moves[bestMoveIndex];
}