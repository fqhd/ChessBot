#include "bot.hpp"


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

int main() {

    std::cout << "Count Material: " << (count_material_test() ? "Pass" : "Fail") << std::endl;

}
