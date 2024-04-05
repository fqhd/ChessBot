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

	Move move = FindBestMove(board, searchDepth);

	std::cout << uci::moveToSan(board, move) << std::endl;

    return 0;
}