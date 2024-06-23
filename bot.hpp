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

static bool compareMoves(Move a, Move b) {
	return a.score() > b.score();
}

const int QUIESCENCE_SEARCH_DEPTH = -1;
struct TTEntry {
	int eval;
	int depth;
};

class ChessBot {
public:

	ChessBot(const std::string& fen, int depth): m_board(fen), m_depth(depth) {}
	
	std::string FindBestMove() {
		Movelist moves;
		movegen::legalmoves(moves, m_board);
		OrderMoves(moves);

		std::vector<int> evaluations;

		for (int i = 0; i < moves.size(); i++) {
			m_board.makeMove(moves[i]);
			evaluations.push_back(-Search(m_depth, 0, -KING_VALUE, KING_VALUE));
			m_board.unmakeMove(moves[i]);
		}

		int bestMoveEval = -KING_VALUE;
		int bestMoveIndex = 0;
		for (int i = 0; i < evaluations.size(); i++) {
			if (evaluations[i] > bestMoveEval) {
				bestMoveEval = evaluations[i];
				bestMoveIndex = i;
			}
		}

		return uci::moveToSan(m_board, moves[bestMoveIndex]);
	}

private:
	
	Board m_board;
	int m_depth = 0;
	std::unordered_map<uint64_t, TTEntry> m_transpositions;

	PieceType GetPieceType(Square square) {
		Bitboard squareBit;
		squareBit = squareBit |= (1ULL << square.index());
		uint64_t pawn = (squareBit & m_board.pieces(PieceType::PAWN)).getBits();
		if (pawn) return PieceType::PAWN;

		uint64_t bishop = (squareBit & m_board.pieces(PieceType::BISHOP)).getBits();
		if (bishop) return PieceType::BISHOP;

		uint64_t knight = (squareBit & m_board.pieces(PieceType::KNIGHT)).getBits();
		if (knight) return PieceType::KNIGHT;

		uint64_t rook = (squareBit & m_board.pieces(PieceType::ROOK)).getBits();
		if (rook) return PieceType::ROOK;

		uint64_t queen = (squareBit & m_board.pieces(PieceType::QUEEN)).getBits();
		if (queen) return PieceType::QUEEN;

		uint64_t king = (squareBit & m_board.pieces(PieceType::KING)).getBits();
		if (king) return PieceType::KING;

		return PieceType::NONE;
	}

	int CalculateMoveScore(Move move) {
		int moveScoreGuess = 0;
		PieceType fromType = GetPieceType(move.from());
		PieceType toType = GetPieceType(move.to());

		if (toType != PieceType::NONE) {
			moveScoreGuess = 10 * GetPieceValue(toType) - GetPieceValue(fromType);
		}

		if (move == Move::PROMOTION) {
			moveScoreGuess += GetPieceValue(move.promotionType());
		}

		if (m_board.isAttacked(move.to(), ~m_board.sideToMove())) {
			moveScoreGuess -= GetPieceValue(fromType);
		}

		return moveScoreGuess;
	}


	int Evaluate() {
		int whiteEval = CountMaterial(Color::WHITE);
		int blackEval = CountMaterial(Color::BLACK);

		int evaluation = whiteEval - blackEval;
		
		if (m_board.sideToMove() == Color::WHITE) {
			return evaluation;
		}
		else {
			return -evaluation;
		}
	}

	int CountMaterial(Color side) {
		Bitboard us = m_board.us(side);
		int pawns = (m_board.pieces(PieceType::PAWN) & us).count() * PAWN_VALUE;
		int knights = (m_board.pieces(PieceType::KNIGHT) & us).count() * KNIGHT_VALUE;
		int bishops = (m_board.pieces(PieceType::BISHOP) & us).count() * BISHOP_VALUE;
		int rooks = (m_board.pieces(PieceType::ROOK) & us).count() * ROOK_VALUE;
		int queens = (m_board.pieces(PieceType::QUEEN) & us).count() * QUEEN_VALUE;
		return pawns + knights + bishops + rooks + queens;
	}
	
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
			return KING_VALUE;
		}
		return 0;
	}

	void OrderMoves(Movelist& moves) {
		for (auto& move : moves) {
			move.setScore(CalculateMoveScore(move));
		}
		std::sort(moves.begin(), moves.end(), compareMoves);
	}

	int Search(int depth, int ply, int alpha, int beta) {
		auto it = m_transpositions.find(m_board.zobrist());
		if (it != m_transpositions.end() && it->second.depth >= depth) {
			return it->second.eval;
		}
		if (depth == 0) {
			return SearchAllCaptures(alpha, beta);
		}

		Movelist moves;
		movegen::legalmoves(moves, m_board);
		if (moves.size() == 0) {
			if (m_board.inCheck()) {
				int eval = -KING_VALUE + ply;
				m_transpositions[m_board.zobrist()] = { eval, depth };
				return eval;
			}
			m_transpositions[m_board.zobrist()] = { 0, depth };
			return 0;
		}
		OrderMoves(moves);

		for (const auto& move : moves) {
			m_board.makeMove(move);
			int eval = -Search(depth - 1, ply + 1, -beta, -alpha);
			if (eval >= beta) {
				m_transpositions[m_board.zobrist()] = {beta, depth};
				m_board.unmakeMove(move);
				return beta;
			}
			m_board.unmakeMove(move);
			alpha = std::max(alpha, eval);
		}

		m_transpositions[m_board.zobrist()] = { alpha, depth };
		return alpha;
	}

	int SearchAllCaptures(int alpha, int beta) {
		auto it = m_transpositions.find(m_board.zobrist());
		if (it != m_transpositions.end() && it->second.depth == QUIESCENCE_SEARCH_DEPTH) {
			return it->second.eval;
		}

		int evaluation = Evaluate();
		if (evaluation >= beta) {
			m_transpositions[m_board.zobrist()] = { beta, QUIESCENCE_SEARCH_DEPTH };
			return beta;
		}
		alpha = std::max(alpha, evaluation);

		Movelist moves;
		movegen::legalmoves<movegen::MoveGenType::CAPTURE>(moves, m_board);
		OrderMoves(moves);

		for (const auto& move : moves) {
			m_board.makeMove(move);
			evaluation = -SearchAllCaptures(-beta, -alpha);
			if (evaluation >= beta) {
				m_transpositions[m_board.zobrist()] = { beta, QUIESCENCE_SEARCH_DEPTH };
				m_board.unmakeMove(move);
				return beta;
			}
			m_board.unmakeMove(move);
			alpha = std::max(alpha, evaluation);
		}

		m_transpositions[m_board.zobrist()] = { alpha, QUIESCENCE_SEARCH_DEPTH };
		return alpha;
	}

};
