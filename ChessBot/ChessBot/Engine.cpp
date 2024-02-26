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