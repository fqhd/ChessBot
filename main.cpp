#include "bot.hpp"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << find_best_move("1r1q2kr/pp2R1pp/n2pp3/3N4/2P2Q2/3B1N2/PP3PPP/6K1 w - - 0 1", 5) << std::endl;
	std::chrono::duration<double> dur = std::chrono::high_resolution_clock::now() - start;
	std::cout << dur.count() << " seconds" << std::endl;
#else
	if (argc < 3) {
		std::cerr << "Not enough keyword arguments to run the command" << std::endl;
		return -1;
	}
	std::string fen = argv[1];
	int depth = std::stoi(argv[2]);
	std::string move = find_best_move(fen, depth);
	std::cout << move << std::endl;
#endif
    return 0;
}
