#include "bot.hpp"
#include <chrono>

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	auto start = std::chrono::high_resolution_clock::now();
	ChessBot bot("1r1q2kr/pp2R1pp/n2pp3/3N4/2P2Q2/3B1N2/PP3PPP/6K1 w - - 0 1", 5);
	std::cout << bot.FindBestMove() << std::endl;
	std::chrono::duration<double> dur = std::chrono::high_resolution_clock::now() - start;
	std::cout << dur.count() << " seconds" << std::endl;
#else
	if (argc < 3) {
		std::cerr << "Not enough keyword arguments to run the command" << std::endl;
		return -1;
	}
	std::string fen = argv[1];
	int depth = std::stoi(argv[2]);
	ChessBot bot(fen, depth);
	std::string move = bot.FindBestMove();
	std::cout << move << std::endl;
#endif
	
    return 0;
}