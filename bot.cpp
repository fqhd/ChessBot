#include "bot.hpp"

int numPositions = 0;

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
			return -INT_MAX;
		}
		return 0;
	}

	int bestEvaluation = -INT_MAX;
	for(auto& move : moves) {
		cr->PushMove(move);
		int eval = -oldSearch(cr, depth - 1);
		bestEvaluation = std::max(eval, bestEvaluation);
		cr->PopMove(move);
	}

	return bestEvaluation;
}

int search(thc::ChessRules* cr, int depth, int alpha, int beta) {
	if(depth == 0) return evaluate(cr);
	std::vector<thc::Move> moves;
	cr->GenLegalMoveList(moves);
	if(moves.size() == 0) {
		thc::Square s = cr->WhiteToPlay() ? cr->wking_square : cr->bking_square;
		if(cr->AttackedPiece(s)){
			return -INT_MAX;
		}
		return 0;
	}

	for(auto& move : moves) {
		cr->PushMove(move);
		int eval = -search(cr, depth - 1, -beta, -alpha);
		cr->PopMove(move);
		if(eval >= beta) {
			return beta;
		}
		alpha = std::max(alpha, eval);
	}

	return alpha;
}
 
thc::Move findBestMove(thc::ChessRules cr, int depth) {
	std::vector<thc::Move> moves;
	cr.GenLegalMoveList(moves);
	int bestMoveIndex = 0;
	int bestEvaluation = -INT_MAX;
	for(unsigned int i = 0; i < moves.size(); i++) {
		cr.PushMove(moves[i]);
		int evaluation = -search(&cr, depth, -INT_MAX, INT_MAX);
		if (evaluation > bestEvaluation) {
			bestMoveIndex = i;
			bestEvaluation = evaluation;
		}
		cr.PopMove(moves[i]);
	}
	std::cout << "Num Positions Evaluated: " << numPositions << std::endl;
	return moves[bestMoveIndex];
}