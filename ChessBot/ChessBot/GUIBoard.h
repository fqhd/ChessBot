#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Engine.h"
#include "chess.hpp"

class GUIBoard
{
public:

	int create(bool invert);
	void update();
	void draw(const chess::Board& board);
	bool isOpen() const;
	bool sentCommand() const;
	void setSentCommand(bool sc);
	void commandFailed();
	void commandExecuted();
	const std::string& getCommand() const;

private:

	void drawBackground();
	void drawTime(const chess::Board& board);
	void drawPieces(const chess::Board& board);
	void drawPiece(const chess::Board& board, chess::Color color, const chess::PieceType type);
	void drawPlayerTime(float t, float yPos, sf::Color color);
	void drawCommand();

	sf::Clock m_clock;
	sf::Font m_font;
	float m_whiteTime = 0.0f;
	float m_blackTime = 0.0f;
	sf::RenderWindow m_window;
	sf::Texture m_pieceTexture;
	bool m_invert;
	bool m_isOpen;
	bool m_sentCommand;
	sf::Color m_commandColor;
	std::string m_command;
	bool m_lastCommandFailed;

};
