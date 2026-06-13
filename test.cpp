#include "bot.hpp"
#include "chess.hpp"


bool count_material_test() {
    Board b1("r1bqk3/2pppppp/8/8/8/8/PP3PPP/2BQKB2 w q - 0 1"); // Black: 23 White: 20
    Board b2("r1bqk3/2pppppp/2b1b3/8/8/3Q1P2/PP1Q2PP/2BQKB2 b q - 0 1"); // Black: 29 White: 38
    Board b3("8/3k4/8/8/8/8/8/1KQ5 w - - 0 1"); // Black: 0 White: 9
    Board b4("8/3k4/8/8/8/8/8/1KN5 w - - 0 1"); // Black: 0 White: 3
    Board b5("8/3kr3/8/8/8/8/2N5/1KBP4 w - - 0 1"); // Black: 5 White: 7
    Board b6("8/3krq2/8/8/8/8/2NPP3/1KB5 w - - 0 1"); // Black: 14 White: 8

    if (count_material(b1, Color::WHITE) != 20) return false;
    if (count_material(b1, Color::BLACK) != 23) return false;

    if (count_material(b2, Color::WHITE) != 38) return false;
    if (count_material(b2, Color::BLACK) != 29) return false;

    if (count_material(b3, Color::WHITE) != 9) return false;
    if (count_material(b3, Color::BLACK) != 0) return false;

    if (count_material(b4, Color::WHITE) != 3) return false;
    if (count_material(b4, Color::BLACK) != 0) return false;

    if (count_material(b5, Color::WHITE) != 7) return false;
    if (count_material(b5, Color::BLACK) != 5) return false;

    if (count_material(b6, Color::WHITE) != 8) return false;
    if (count_material(b6, Color::BLACK) != 14) return false;

    return true;
}

bool evaluate_test() {
    Board b1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); // 0
    Board b2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1"); // -1
    Board b3("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPP1/RNBQKB1R w KQkq - 0 1"); // -4
    Board b4("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPP1/RNBQKB1R b KQkq - 0 1"); // 4
    Board b5("1n1qkbnr/pppppppp/8/8/8/8/PPPPPPP1/RN2KB1R b KQk - 0 1"); // 8
    Board b6("3k4/8/8/8/3PP3/3K4/8/8 w - - 0 1"); // 2
    Board b7("3k2r1/8/8/8/3PP3/3K4/8/4BB2 b - - 0 1"); // -3
    Board b8("6rq/8/2k5/8/3PP3/3K4/8/4BB2 b - - 0 1"); // 6

    if (evaluate(b1) != 0) return false;
    if (evaluate(b2) != -1) return false;
    if (evaluate(b3) != -4) return false;
    if (evaluate(b4) != 4) return false;
    if (evaluate(b5) != 8) return false;
    if (evaluate(b6) != 2) return false;
    if (evaluate(b7) != -3) return false;
    if (evaluate(b8) != 6) return false;

    return true;
}

bool get_piece_type_test() {
    Board b1("r1b1B2k/pp4pp/2pb4/4q3/8/6P1/PPP4P/R2Q1R1K b - - 0 17");
    Board b2("3Q4/p1p2ppp/4k3/8/5P2/4P3/Prqn2PP/3R1RK1 b - - 0 22");
    Board b3("2r4k/q2b1Q1p/2p2P2/1p2p3/4P3/p2P3R/4N1K1/R7 b - - 0 38");

    if (get_piece_type(b1, Square::underlying::SQ_A1) != PieceType::ROOK) return false;
    if (get_piece_type(b1, Square::underlying::SQ_A2) != PieceType::PAWN) return false;
    if (get_piece_type(b1, Square::underlying::SQ_A8) != PieceType::ROOK) return false;
    if (get_piece_type(b1, Square::underlying::SQ_D1) != PieceType::QUEEN) return false;

    if (get_piece_type(b2, Square::underlying::SQ_B4) != PieceType::NONE) return false;
    if (get_piece_type(b2, Square::underlying::SQ_C7) != PieceType::PAWN) return false;
    if (get_piece_type(b2, Square::underlying::SQ_G5) != PieceType::NONE) return false;
    if (get_piece_type(b2, Square::underlying::SQ_D2) != PieceType::KNIGHT) return false;

    if (get_piece_type(b3, Square::underlying::SQ_D7) != PieceType::BISHOP) return false;
    if (get_piece_type(b3, Square::underlying::SQ_A7) != PieceType::QUEEN) return false;
    if (get_piece_type(b3, Square::underlying::SQ_H8) != PieceType::KING) return false;
    if (get_piece_type(b3, Square::underlying::SQ_H1) != PieceType::NONE) return false;

    return true;
}

bool get_piece_value_test() {
    if (get_piece_value(PieceType::PAWN) > get_piece_value(PieceType::BISHOP)) return false;
    if (get_piece_value(PieceType::PAWN) > get_piece_value(PieceType::KNIGHT)) return false;
    if (get_piece_value(PieceType::KNIGHT) > get_piece_value(PieceType::ROOK)) return false;
    if (get_piece_value(PieceType::BISHOP) > get_piece_value(PieceType::ROOK)) return false;
    if (get_piece_value(PieceType::ROOK) > get_piece_value(PieceType::QUEEN)) return false;
    if (get_piece_value(PieceType::QUEEN) > get_piece_value(PieceType::KING)) return false;

    return true;
}

int main() {

    std::cout << "Count Material: " << (count_material_test() ? "Pass" : "Fail") << std::endl;
    std::cout << "Evaluate: " << (evaluate_test() ? "Pass" : "Fail") << std::endl;
    std::cout << "Get Piece Type: " << (get_piece_type_test() ? "Pass" : "Fail") << std::endl;
    std::cout << "Get Piece Value: " << (get_piece_value_test() ? "Pass" : "Fail") << std::endl;

}
