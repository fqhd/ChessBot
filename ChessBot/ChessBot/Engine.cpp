#include "Engine.h"

using namespace chess;

const std::string& Engine::getPreviousMove() const {
	return m_previousMove;
}

const chess::Board& Engine::getBoard() const {
	return m_board;
}

bool Engine::isUserPlayingBlack() const {
	return m_userIsBlack;
}

Turn Engine::getTurn() const {
	return m_turn;
}

void Engine::makeMove() {
	Movelist moves;

	movegen::legalmoves(moves, m_board);

	m_board.makeMove(moves[rand() % moves.size()]);
}