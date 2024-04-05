#include <iostream>
#include "bot.hpp"
#include "testlib.hpp"

bool getPieceValueTest() {
	if (getPieceValue(PieceType::PAWN) != PAWN_VALUE) {
		return true;
	}
	if (getPieceValue(PieceType::BISHOP) != BISHOP_VALUE) {
		return true;
	}
	if (getPieceValue(PieceType::KNIGHT) != KNIGHT_VALUE) {
		return true;
	}
	if (getPieceValue(PieceType::ROOK) != ROOK_VALUE) {
		return true;
	}
	if (getPieceValue(PieceType::QUEEN) != QUEEN_VALUE) {
		return true;
	}
	if (getPieceValue(PieceType::KING) != KING_VALUE) {
		return true;
	}
	if (getPieceValue(PieceType::NONE) != 0) {
		return true;
	}

	return false;
}

bool countMaterialTest() {
	Board board("rnb1k2r/pppqb1pp/3p1n2/8/8/4Q1P1/PPPP1P1P/RNB1KB1R b KQkq - 0 1");
	if(countMaterial(board, Color::WHITE) != 35) {
		return true;
	}
	if(countMaterial(board, Color::BLACK) != 37) {
		return true;
	}
	board.setFen("4n1k1/3r4/2Q2pPP/pPn1N3/5p1K/2Pb4/qpP5/6R1 w - - 0 1");
	if(countMaterial(board, Color::WHITE) != 22) {
		return true;
	}
	if(countMaterial(board, Color::BLACK) != 27) {
		return true;
	}
	board.setFen("8/7R/1K2P3/8/8/P7/4P3/2k5 w - - 0 1");
	if(countMaterial(board, Color::WHITE) != 8) {
		return true;
	}
	if(countMaterial(board, Color::BLACK) != 0) {
		return true;
	}
	return false;
}

bool evaluateTest() {
	if(evaluate(Board("rnb1k2r/pppqb1pp/3p1n2/8/8/4Q1P1/PPPP1P1P/RNB1KB1R b KQkq - 0 1")) != 2) {
		return true;
	}
	if(evaluate(Board("rnb1k2r/pppqb1pp/3p1n2/8/8/4Q1P1/PPPP1P1P/RNB1KB1R w KQkq - 0 1")) != -2) {
		return true;
	}
	if(evaluate(Board("4n1k1/3r4/2Q2pPP/pPn1N3/5p1K/2Pb4/qpP5/6R1 w - - 0 1")) != -5) {
		return true;
	}
	if(evaluate(Board("4n1k1/3r4/2Q2pPP/pPn1N3/5p1K/2Pb4/qpP5/6R1 b - - 0 1")) != 5) {
		return true;
	}
	if(evaluate(Board("8/7R/1K2P3/8/8/P7/4P3/2k5 w - - 0 1")) != 8) {
		return true;
	}
	if(evaluate(Board("8/7R/1K2P3/8/8/P7/4P3/2k5 b - - 0 1")) != -8) {
		return true;
	}
	return false;
}

bool getPieceTypeTest() {
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_5, File::FILE_B)) != PieceType::BISHOP) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_6, File::FILE_B)) != PieceType::NONE) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_6, File::FILE_C)) != PieceType::PAWN) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_6, File::FILE_D)) != PieceType::NONE) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 b - - 0 1"), Square(Rank::RANK_6, File::FILE_D)) != PieceType::NONE) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_8, File::FILE_A)) != PieceType::KING) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_8, File::FILE_E)) != PieceType::KING) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_7, File::FILE_G)) != PieceType::ROOK) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_1, File::FILE_G)) != PieceType::KNIGHT) {
		return true;
	}
	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_1, File::FILE_H)) != PieceType::NONE) {
		return true;
	}
	if(getPieceType(Board("4k2q/p5p1/1p1pK3/3R2p1/2n4b/QP1b3P/P1RPP3/8 w - - 0 1"), Square(Rank::RANK_3, File::FILE_A)) != PieceType::QUEEN) {
		return true;
	}
	if(getPieceType(Board("4k2q/p5p1/1p1pK3/3R2p1/2n4b/QP1b3P/P1RPP3/8 w - - 0 1"), Square(Rank::RANK_8, File::FILE_H)) != PieceType::QUEEN) {
		return true;
	}
	return false;
}

int main() {

	test("Get piece value", getPieceValueTest);
	test("Count material", countMaterialTest);
	test("Evaluate position", evaluateTest);
	test("Get piece type", getPieceTypeTest);
	
	printResults();

	return 0;
}