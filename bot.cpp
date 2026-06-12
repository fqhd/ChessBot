#include "bot.hpp"

bool compare_moves(Move a, Move b) {
	return a.score() > b.score();
}

std::string find_best_move(const std::string& fen, int depth) {
    std::unordered_map<uint64_t, TTEntry> transpositions;
    Board board(fen);

    Movelist moves;
	movegen::legalmoves(moves, board);
	order_moves(board, moves);

	std::vector<int> evaluations;

	for (int i = 0; i < moves.size(); i++) {
		board.makeMove(moves[i]);
		evaluations.push_back(-search(board, transpositions, depth, 0, -KING_VALUE, KING_VALUE));
		board.unmakeMove(moves[i]);
	}

	int best_move_eval = -KING_VALUE;
	int best_move_index = 0;
	for (int i = 0; i < evaluations.size(); i++) {
		if (evaluations[i] > best_move_eval) {
			best_move_eval = evaluations[i];
			best_move_index = i;
		}
	}

	return uci::moveToSan(board, moves[best_move_index]);
}

void order_moves(Board board, Movelist& moves) {
	for (auto& move : moves) {
		move.setScore(score_move(board, move));
	}
	std::sort(moves.begin(), moves.end(), compare_moves);
}

int score_move(Board board, Move move) {
	int move_score_guess = 0;
	PieceType fromType = get_piece_type(board, move.from());
	PieceType toType = get_piece_type(board, move.to());

	if (toType != PieceType::NONE) {
		move_score_guess = 10 * get_piece_value(toType) - get_piece_value(fromType);
	}

	if (move == Move::PROMOTION) {
		move_score_guess += get_piece_value(move.promotionType());
	}

	if (board.isAttacked(move.to(), ~board.sideToMove())) {
		move_score_guess -= get_piece_value(fromType);
	}

	return move_score_guess;
}

PieceType get_piece_type(Board board, Square square) {
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

int evaluate(Board board) {
	int whiteEval = count_material(board, Color::WHITE);
	int blackEval = count_material(board, Color::BLACK);

	int evaluation = whiteEval - blackEval;

	if (board.sideToMove() == Color::WHITE) {
		return evaluation;
	}
	else {
		return -evaluation;
	}
}

int count_material(Board board, Color side) {
	Bitboard us = board.us(side);
	int pawns = (board.pieces(PieceType::PAWN) & us).count() * PAWN_VALUE;
	int knights = (board.pieces(PieceType::KNIGHT) & us).count() * KNIGHT_VALUE;
	int bishops = (board.pieces(PieceType::BISHOP) & us).count() * BISHOP_VALUE;
	int rooks = (board.pieces(PieceType::ROOK) & us).count() * ROOK_VALUE;
	int queens = (board.pieces(PieceType::QUEEN) & us).count() * QUEEN_VALUE;
	return pawns + knights + bishops + rooks + queens;
}

int get_piece_value(PieceType type) {
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

int quiescence_search(Board& board, std::unordered_map<uint64_t, TTEntry>& transpositions, int alpha, int beta) {
	auto it = transpositions.find(board.zobrist());
	if (it != transpositions.end() && it->second.depth == QUIESCENCE_SEARCH_DEPTH) {
		return it->second.eval;
	}

	int evaluation = evaluate(board);
	if (evaluation >= beta) {
		transpositions[board.zobrist()] = { beta, QUIESCENCE_SEARCH_DEPTH };
		return beta;
	}
	alpha = std::max(alpha, evaluation);

	Movelist moves;
	movegen::legalmoves<movegen::MoveGenType::CAPTURE>(moves, board);
	order_moves(board, moves);

	for (const auto& move : moves) {
		board.makeMove(move);
		evaluation = -quiescence_search(board, transpositions, -beta, -alpha);
		if (evaluation >= beta) {
			transpositions[board.zobrist()] = { beta, QUIESCENCE_SEARCH_DEPTH };
			board.unmakeMove(move);
			return beta;
		}
		board.unmakeMove(move);
		alpha = std::max(alpha, evaluation);
	}

	transpositions[board.zobrist()] = { alpha, QUIESCENCE_SEARCH_DEPTH };
	return alpha;
}

int search(Board& board, std::unordered_map<uint64_t, TTEntry>& transpositions, int depth, int ply, int alpha, int beta) {
	auto it = transpositions.find(board.zobrist());
	if (it != transpositions.end() && it->second.depth >= depth) {
		return it->second.eval;
	}
	if (depth == 0) {
		return quiescence_search(board, transpositions, alpha, beta);
	}

	Movelist moves;
	movegen::legalmoves(moves, board);
	if (moves.size() == 0) {
		if (board.inCheck()) {
			int eval = -KING_VALUE + ply;
			transpositions[board.zobrist()] = { eval, depth };
			return eval;
		}
		transpositions[board.zobrist()] = { 0, depth };
		return 0;
	}
	order_moves(board, moves);

	for (const auto& move : moves) {
		board.makeMove(move);
		int eval = -search(board, transpositions, depth - 1, ply + 1, -beta, -alpha);
		if (eval >= beta) {
			transpositions[board.zobrist()] = {beta, depth};
			board.unmakeMove(move);
			return beta;
		}
		board.unmakeMove(move);
		alpha = std::max(alpha, eval);
	}

	transpositions[board.zobrist()] = { alpha, depth };
	return alpha;
}
