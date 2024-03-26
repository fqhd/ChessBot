#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Engine.h"
#include "chess.hpp"

class GUIBoard
{
public:

	int create(bool invert);
	void draw(const chess::Board& board);
	bool isOpen();
	// void makeMove(const std::string& move);

private:

	void drawBackground();
	void drawTime();
	void drawPieces(const chess::Board& board);
	void drawPiece(const chess::Board& board, chess::Color color, const chess::PieceType type);
	void drawPlayerTime(float t, float yPos, bool userMove);

	sf::Clock m_clock;
	sf::Font m_font;
	float m_whiteTime = 0.0f;
	float m_blackTime = 0.0f;
	sf::RenderWindow m_window;
	sf::Texture m_pieceTexture;
	bool m_invert;

};

