#include "bot.hpp"
#include <algorithm>

int numPositions = 0;

struct ScoredMove {
	ScoredMove(const thc::Move& m, int s) {
		move = m;
		score = s;
	}
	thc::Move move;
	int score;
};

int countMaterial(thc::ChessRules* cr, bool white) {
	int material = 0;
	if(white) {
		// Look for uppercase
		for(int i = 0; i < 64; i++){
			if(cr->squares[i] == 'P') {
				material += PAWN_VALUE;
			}else if(cr->squares[i] == 'R') {
				material += ROOK_VALUE;
			}else if(cr->squares[i] == 'B') {
				material += BISHOP_VALUE;
			}else if(cr->squares[i] == 'N') {
				material += KNIGHT_VALUE;
			}else if(cr->squares[i] == 'Q') {
				material += QUEEN_VALUE;
			}
		}
	}else{
		// Look for lowercase
		for(int i = 0; i < 64; i++){
			if(cr->squares[i] == 'p') {
				material += PAWN_VALUE;
			}else if(cr->squares[i] == 'r') {
				material += ROOK_VALUE;
			}else if(cr->squares[i] == 'b') {
				material += BISHOP_VALUE;
			}else if(cr->squares[i] == 'n') {
				material += KNIGHT_VALUE;
			}else if(cr->squares[i] == 'q') {
				material += QUEEN_VALUE;
			}
		}
	}
	return material;
}

int evaluate(thc::ChessRules* cr) {
	numPositions++;
	int whiteMaterial = countMaterial(cr, true);
	int blackMaterial = countMaterial(cr, false);

	int evaluation = whiteMaterial - blackMaterial;

	if(cr->WhiteToPlay()) return evaluation;
	return -evaluation;
}

int oldSearch(thc::ChessRules* cr, int depth) {
	if(depth == 0) return evaluate(cr);
	std::vector<thc::Move> moves;
	cr->GenLegalMoveList(moves);
	if(moves.size() == 0) {
		thc::Square s = cr->WhiteToPlay() ? cr->wking_square : cr->bking_square;
		if(cr->AttackedPiece(s)){
			return -KING_VALUE;
		}
		return 0;
	}

	int bestEvaluation = -KING_VALUE;
	for(auto& move : moves) {
		cr->PushMove(move);
		int eval = -oldSearch(cr, depth - 1);
		bestEvaluation = std::max(eval, bestEvaluation);
		cr->PopMove(move);
	}

	return bestEvaluation;
}

int getPieceValue(char piece) {
	switch(piece) {
		case 'p':
			return PAWN_VALUE;
		break;
		case 'b':
			return BISHOP_VALUE;
		break;
		case 'n':
			return KNIGHT_VALUE;
		break;
		case 'r':
			return ROOK_VALUE;
		break;
		case 'q':
			return QUEEN_VALUE;
		break;
		case 'k':
			return KING_VALUE;
		break;
		case 'P':
			return PAWN_VALUE;
		break;
		case 'B':
			return BISHOP_VALUE;
		break;
		case 'N':
			return KNIGHT_VALUE;
		break;
		case 'R':
			return ROOK_VALUE;
		break;
		case 'Q':
			return QUEEN_VALUE;
		break;
		case 'K':
			return KING_VALUE;
		break;
	}
	return 0;
}

int calculateScore(thc::ChessRules* cr, thc::Move move) {
	int moveScoreGuess = 0;

	char movePieceType = cr->squares[move.src];
	char capturePieceType = cr->squares[move.dst];

	if (move.capture != ' ') {
		moveScoreGuess += 10 * getPieceValue(capturePieceType) - getPieceValue(movePieceType);
	}

	if(move.special == thc::SPECIAL_PROMOTION_BISHOP) {
		moveScoreGuess += BISHOP_VALUE;
	}else if(move.special == thc::SPECIAL_PROMOTION_KNIGHT) {
		moveScoreGuess += KNIGHT_VALUE;
	}else if(move.special == thc::SPECIAL_PROMOTION_ROOK) {
		moveScoreGuess += ROOK_VALUE;
	}else if(move.special == thc::SPECIAL_PROMOTION_QUEEN) {
		moveScoreGuess += QUEEN_VALUE;
	}

	if(cr->AttackedPiece(move.dst)) {
		moveScoreGuess -= getPieceValue(movePieceType);
	}

	return moveScoreGuess;
}

bool compareMoves(ScoredMove a, ScoredMove b) {
	return a.score > b.score;
}

int search(thc::ChessRules* cr, int depth, int alpha, int beta) {
	if(depth == 0) return evaluate(cr);
	std::vector<thc::Move> unorderedMoves;
	cr->GenLegalMoveList(unorderedMoves);
	if(unorderedMoves.size() == 0) {
		thc::Square s = cr->WhiteToPlay() ? cr->wking_square : cr->bking_square;
		if(cr->AttackedPiece(s)){
			return -KING_VALUE;
		}
		return 0;
	}
	std::vector<ScoredMove> moves;
	for(auto& move : unorderedMoves) {
		moves.emplace_back(move, calculateScore(cr, move));
	}
	std::sort(moves.begin(), moves.end(), compareMoves);

	for(auto& move : moves) {
		cr->PushMove(move.move);
		int eval = -search(cr, depth - 1, -beta, -alpha);
		cr->PopMove(move.move);
		if(eval >= beta) {
			return beta;
		}
		alpha = std::max(alpha, eval);
	}

	return alpha;
}
 
thc::Move findBestMove(thc::ChessRules cr, int depth) {
	std::vector<thc::Move> unorderedMoves;
	cr.GenLegalMoveList(unorderedMoves);
	std::vector<ScoredMove> moves;
	for(auto& move : unorderedMoves) {
		moves.emplace_back(move, calculateScore(&cr, move));
	}
	std::sort(moves.begin(), moves.end(), compareMoves);
	int bestMoveIndex = 0;
	int bestEvaluation = -KING_VALUE;
	for(unsigned int i = 0; i < moves.size(); i++) {
		cr.PushMove(moves[i].move);
		int evaluation = -search(&cr, depth, -KING_VALUE, KING_VALUE);
		if (evaluation > bestEvaluation) {
			bestMoveIndex = i;
			bestEvaluation = evaluation;
		}
		cr.PopMove(moves[i].move);
	}
	std::cout << "Num Positions Evaluated: " << numPositions << std::endl;
	return moves[bestMoveIndex].move;
}