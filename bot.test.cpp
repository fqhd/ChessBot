#include "bot.hpp"
#include "testlib.hpp"

// bool getPieceValueTest() {
// 	if (getPieceValue(PieceType::PAWN) != PAWN_VALUE) {
// 		return true;
// 	}
// 	if (getPieceValue(PieceType::BISHOP) != BISHOP_VALUE) {
// 		return true;
// 	}
// 	if (getPieceValue(PieceType::KNIGHT) != KNIGHT_VALUE) {
// 		return true;
// 	}
// 	if (getPieceValue(PieceType::ROOK) != ROOK_VALUE) {
// 		return true;
// 	}
// 	if (getPieceValue(PieceType::QUEEN) != QUEEN_VALUE) {
// 		return true;
// 	}
// 	if (getPieceValue(PieceType::KING) != INT_MAX) {
// 		return true;
// 	}
// 	if (getPieceValue(PieceType::NONE) != 0) {
// 		return true;
// 	}

// 	return false;
// }

bool countMaterialTest() {
	thc::ChessRules cr;
	cr.Forsyth("rnb1k2r/pppqb1pp/3p1n2/8/8/4Q1P1/PPPP1P1P/RNB1KB1R b KQkq - 0 1");

	if(countMaterial(&cr, true) != 35) {
		return true;
	}
	if(countMaterial(&cr, false) != 37) {
		return true;
	}

	cr.Forsyth("4n1k1/3r4/2Q2pPP/pPn1N3/5p1K/2Pb4/qpP5/6R1 w - - 0 1");
	if(countMaterial(&cr, true) != 22) {
		return true;
	}
	if(countMaterial(&cr, false) != 27) {
		return true;
	}

	cr.Forsyth("8/7R/1K2P3/8/8/P7/4P3/2k5 w - - 0 1");
	if(countMaterial(&cr, true) != 8) {
		return true;
	}
	if(countMaterial(&cr, false) != 0) {
		return true;
	}
	return false;
}

bool evaluateTest() {
	thc::ChessRules cr;
	cr.Forsyth("rnb1k2r/pppqb1pp/3p1n2/8/8/4Q1P1/PPPP1P1P/RNB1KB1R b KQkq - 0 1");
	if(evaluate(&cr) != 2) {
		return true;
	}

	cr.Forsyth("rnb1k2r/pppqb1pp/3p1n2/8/8/4Q1P1/PPPP1P1P/RNB1KB1R w KQkq - 0 1");
	if(evaluate(&cr) != -2) {
		return true;
	}

	cr.Forsyth("4n1k1/3r4/2Q2pPP/pPn1N3/5p1K/2Pb4/qpP5/6R1 w - - 0 1");
	if(evaluate(&cr) != -5) {
		return true;
	}

	cr.Forsyth("4n1k1/3r4/2Q2pPP/pPn1N3/5p1K/2Pb4/qpP5/6R1 b - - 0 1");
	if(evaluate(&cr) != 5) {
		return true;
	}

	cr.Forsyth("8/7R/1K2P3/8/8/P7/4P3/2k5 w - - 0 1");
	if(evaluate(&cr) != 8) {
		return true;
	}

	cr.Forsyth("8/7R/1K2P3/8/8/P7/4P3/2k5 b - - 0 1");
	if(evaluate(&cr) != -8) {
		return true;
	}
	return false;
}

// bool getPieceTypeTest() {
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_5, File::FILE_B)) != PieceType::BISHOP) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_6, File::FILE_B)) != PieceType::NONE) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_6, File::FILE_C)) != PieceType::PAWN) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_6, File::FILE_D)) != PieceType::NONE) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 b - - 0 1"), Square(Rank::RANK_6, File::FILE_D)) != PieceType::NONE) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_8, File::FILE_A)) != PieceType::KING) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_8, File::FILE_E)) != PieceType::KING) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_7, File::FILE_G)) != PieceType::ROOK) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_1, File::FILE_G)) != PieceType::KNIGHT) {
// 		return true;
// 	}
// 	if(getPieceType(Board("K3k3/6rP/2p1P2B/1BN1N3/4PP1P/6R1/5b2/6n1 w - - 0 1"), Square(Rank::RANK_1, File::FILE_H)) != PieceType::NONE) {
// 		return true;
// 	}
// 	if(getPieceType(Board("4k2q/p5p1/1p1pK3/3R2p1/2n4b/QP1b3P/P1RPP3/8 w - - 0 1"), Square(Rank::RANK_3, File::FILE_A)) != PieceType::QUEEN) {
// 		return true;
// 	}
// 	if(getPieceType(Board("4k2q/p5p1/1p1pK3/3R2p1/2n4b/QP1b3P/P1RPP3/8 w - - 0 1"), Square(Rank::RANK_8, File::FILE_H)) != PieceType::QUEEN) {
// 		return true;
// 	}
// 	return false;
// }

bool searchTest() {
	thc::ChessRules cr;
	cr.Forsyth("4k3/5qp1/2nbPr1P/3p4/4n1P1/1Q2P3/2p5/6KB w - - 0 1");
	if(oldSearch(&cr, 3) != search(&cr, 3, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("5B2/2k5/bp1R3n/R1pK1P1p/B1P5/2P4P/8/2r5 w - - 0 1");
	if(oldSearch(&cr, 3) != search(&cr, 3, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("R3R1B1/p1p4p/r1P3k1/b1K3B1/1N6/p6P/1p6/8 w - - 0 1");
	if(oldSearch(&cr, 3) != search(&cr, 3, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("4r3/4pP1K/5P2/2p2p2/pp2p3/p6b/6QB/2k1N3 w - - 0 1");
	if(oldSearch(&cr, 3) != search(&cr, 3, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("6r1/1p4Qn/1bP2N2/2pN1P2/8/1p1P3k/1B1R4/3K4 w - - 0 1");
	if(oldSearch(&cr, 3) != search(&cr, 3, -INT_MAX, INT_MAX)) {
		return true;
	}

	cr.Forsyth("3R2Q1/B6K/8/N1PR3P/4n3/2k5/1b1pP1B1/qr6 w - - 0 1");
	if(oldSearch(&cr, 4) != search(&cr, 4, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("2R2n2/5b1Q/rP5P/3k2P1/7q/2Pp4/1KPP4/6n1 w - - 0 1");
	if(oldSearch(&cr, 4) != search(&cr, 4, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("4q2r/8/P6Q/1P1P3p/kN4P1/2b2bp1/2KB4/1B6 w - - 0 1");
	if(oldSearch(&cr, 4) != search(&cr, 4, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("2R2R2/6K1/p2pPPp1/4N3/3p3p/1P1p2kb/2p5/8 w - - 0 1");
	if(oldSearch(&cr, 4) != search(&cr, 4, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("7N/2Pp2B1/6P1/5pr1/PqP4k/1p5p/1R3K2/Q7 w - - 0 1");
	if(oldSearch(&cr, 4) != search(&cr, 4, -INT_MAX, INT_MAX)) {
		return true;
	}

	cr.Forsyth("n2R4/1qrB3P/4K3/k7/8/p3R3/2pP2pp/B4N2 w - - 0 1");
	if(oldSearch(&cr, 5) != search(&cr, 5, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("3n1nb1/4Pp2/8/4R3/8/p3pPpP/pp5b/2k4K w - - 0 1");
	if(oldSearch(&cr, 5) != search(&cr, 5, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("R1b5/6P1/1NN1nK2/r4B1q/1P6/1pk1P1b1/3p4/8 w - - 0 1");
	if(oldSearch(&cr, 5) != search(&cr, 5, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("8/5k1p/1K3P2/1n2R1r1/Q1qNr3/1p6/1p4p1/3b2R1 w - - 0 1");
	if(oldSearch(&cr, 5) != search(&cr, 5, -INT_MAX, INT_MAX)) {
		return true;
	}
	cr.Forsyth("2K5/1pBnPP2/1ppp4/4P2p/2p5/k3b3/4r3/7N w - - 0 1");
	if(oldSearch(&cr, 5) != search(&cr, 5, -INT_MAX, INT_MAX)) {
		return true;
	}
	return false;
}

int main() {

	test("Count material", countMaterialTest);
	test("Evaluate position", evaluateTest);
	test("Search conforms with unoptimized DFS", searchTest);
	
	printResults();

	return 0;
}