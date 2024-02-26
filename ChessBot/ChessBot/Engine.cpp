#include "Engine.h"

using namespace chess;

const chess::Board& Engine::getBoard() const {
	return m_board;
}

void Engine::makeMove() {
	Movelist moves;

	movegen::legalmoves(moves, m_board);

	m_board.makeMove(moves[rand() % moves.size()]);
}

void Engine::playMove(const std::string& move) {
	Movelist moves;

	movegen::legalmoves(moves, m_board);

	std::vector<std::string> stringMoves;

	for (const auto& m : moves) {
		stringMoves.push_back(uci::moveToSan(m_board, m));
	}

	size_t moveIndex = 0;
	bool canPlay = false;
	for (size_t i = 0; i < stringMoves.size(); i++) {
		if (stringMoves[i] == move) {
			canPlay = true;
			moveIndex = i;
		}
	}

	if (canPlay) {
		m_board.makeMove(moves[moveIndex]);
	}
	else {
		std::cout << "Cannot make that move... please try again" << std::endl;
	}
}