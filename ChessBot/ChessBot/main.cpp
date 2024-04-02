#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <climits>
#include "chess.hpp"

using namespace chess;

const int PAWN_VALUE = 1;
const int KNIGHT_VALUE = 3;
const int BISHOP_VALUE = 3;
const int ROOK_VALUE = 5;
const int QUEEN_VALUE = 9;
const int KING_VALUE = 1000;

static int getPieceValue(PieceType type) {
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
	return 1000;
}

static int countMaterial(const Board& board, Color side) {
	Bitboard us = board.us(side);
	int pawns = (board.pieces(PieceType::PAWN) & us).count() * PAWN_VALUE;
	int knights = (board.pieces(PieceType::KNIGHT) & us).count() * KNIGHT_VALUE;
	int bishops = (board.pieces(PieceType::BISHOP) & us).count() * BISHOP_VALUE;
	int rooks = (board.pieces(PieceType::ROOK) & us).count() * ROOK_VALUE;
	int queens = (board.pieces(PieceType::QUEEN) & us).count() * QUEEN_VALUE;
	return pawns + knights + bishops + rooks + queens;
}

static uint64_t numEvaluations = 0;
static uint64_t avgNumberOfLegalMoves = 0;
static uint64_t numLegalMovesDivisor = 0;

static int evaluate(const Board& board) {
	numEvaluations++;
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

static bool compareMoves(Move a, Move b) {
	return a.score() > b.score();
}

static PieceType getPieceType(const Board& board, Square square) {
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

	return PieceType::NONE;
}

static int calculateMoveScore(const Board& board, Move move) {
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


static int search(Board board, int depth, int alpha, int beta) {
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

	avgNumberOfLegalMoves += moves.size();
	numLegalMovesDivisor++;

	int bestEval = -KING_VALUE;

	for (const auto& move : moves) {
		board.makeMove(move);
		int eval = -search(board, depth - 1, -beta, -alpha);
		board.unmakeMove(move);
		if (eval >= beta) {
			return beta;
		}
		if (eval > alpha) alpha = eval;
	}

	return alpha;
}

static void worker(const Board& board, int searchDepth, int* result) {
	int evaluation = -search(board, searchDepth, -KING_VALUE, KING_VALUE);
	*result = evaluation;
}

static Move findBestMove(Board board, int searchDepth) {
	numEvaluations = 0;

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

#ifdef _DEBUG
	std::cout << "Num Positions Searched: " << numEvaluations << std::endl;
	std::cout << "Num Legal Moves Avg: " << avgNumberOfLegalMoves / numLegalMovesDivisor << std::endl;
#endif

	return moves[bestMoveIndex];
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		std::cerr << "Not enough keyword arguments to run the command" << std::endl;
		return -1;
	}

	std::string fen = argv[1];
	int searchDepth = std::stoi(argv[2]);

#ifdef _DEBUG
	std::cout << "Fen: " << fen << std::endl;
	std::cout << "Depth: " << searchDepth << std::endl;
#endif

	Board board(argv[1]);

	Move move = findBestMove(board, searchDepth);

	std::cout << uci::moveToSan(board, move) << std::endl;

    return 0;
}