#pragma once
#include "chess.hpp"


class Engine
{
public:

	const chess::Board& getBoard() const;
	void makeMove();
	void playMove(const std::string& move);

private:

	chess::Board m_board;

};

