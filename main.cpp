#include "bot.hpp"

int main(int argc, char* argv[])
{
	if (argc < 3) {
		std::cerr << "Not enough keyword arguments to run the command" << std::endl;
		return -1;
	}

	std::string fen = argv[1];
	int searchDepth = std::stoi(argv[2]);

	Board board(fen);

	Move move = findBestMove(board, searchDepth);

	std::cout << uci::moveToSan(board, move) << std::endl;

#ifdef _DEBUG
	std::cout << "Fen: " << fen << std::endl;
	std::cout << "Depth: " << searchDepth << std::endl;
	std::cout << "Num Positions Evaluated: " << getNumPositionsEvaluated() << std::endl;
#endif

    return 0;
}