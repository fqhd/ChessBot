#pragma once
#include "chess.hpp"


class Engine
{
public:

	const chess::Board& getBoard() const;
	void makeMove();


private:

	chess::Board m_board;

};

