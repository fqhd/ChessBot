#pragma once
#include "chess.hpp"


class Engine
{
public:

	Engine();
	const chess::Board& getBoard() const;
	void makeMove();
	int playMove(const std::string& move);

private:

	chess::Board m_board;

};

