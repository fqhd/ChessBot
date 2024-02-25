#pragma once
#include "chess.hpp"

enum class Turn {
	USER,
	BOT
};

class Engine
{
public:

	const std::string& getPreviousMove() const;
	const chess::Board& getBoard() const;
	bool isUserPlayingBlack() const;
	Turn getTurn() const;
	void makeMove();


private:
	bool m_userIsBlack;
	Turn m_turn;
	std::string m_previousMove;
	chess::Board m_board;
};

