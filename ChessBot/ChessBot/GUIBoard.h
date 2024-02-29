#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Engine.h"
#include "chess.hpp"

class GUIBoard
{
public:

	int create();
	void draw(const chess::Board& board, chess::Color userColor);
	bool isOpen();
	// void makeMove(const std::string& move);

private:

	void drawBackground();
	void drawTime();
	void drawPieces(const chess::Board& board, chess::Color userColor);
	void drawPiece(const chess::Board& board, chess::Color userColor, const chess::PieceType type, bool us);
	void drawPlayerTime(float t, float yPos, bool userMove);

	sf::Clock m_clock;
	sf::Font m_font;
	float m_whiteTime = 0.0f;
	float m_blackTime = 0.0f;
	sf::RenderWindow m_window;
	sf::Texture m_pieceTexture;

};

