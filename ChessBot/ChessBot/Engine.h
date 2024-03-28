#pragma once
#include "chess.hpp"


class Engine
{
public:

	Engine();
	const chess::Board& getBoard() const;
	void makeMove();
	void playMove();

private:

	chess::Board m_board;

};

