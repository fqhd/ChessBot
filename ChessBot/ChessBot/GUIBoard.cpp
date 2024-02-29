#include "GUIBoard.h"
#include <iostream>

const int TILE_SIZE = 128;

int GUIBoard::create() {
	sf::VideoMode mode;
	mode.width = TILE_SIZE * 14;
	mode.height = TILE_SIZE * 8;
	m_window.create(mode, "ChessBot", sf::Style::Default);
	m_window.setVerticalSyncEnabled(true);
	m_whiteTime = 300.0f;
	m_blackTime = 300.0f;
	if (!m_font.loadFromFile("Roboto-Regular.ttf")) {
		std::cerr << "Failed to load roboto font" << std::endl;
		return -1;
	}
	if (!m_pieceTexture.loadFromFile("pieces.png")) {
		std::cerr << "Failed to load the pieces texture" << std::endl;
		return -1;
	}
	m_pieceTexture.setSmooth(true);
	return 0;
}

void GUIBoard::draw(const chess::Board& board, chess::Color userColor) {
	m_window.clear(sf::Color(31, 31, 31));
	sf::Event e;
	m_window.pollEvent(e);
	switch (e.type) {
	case sf::Event::Closed:
		m_window.close();
		break;
	}

	// If its white to move
	m_whiteTime -= m_clock.getElapsedTime().asSeconds();
	m_clock.restart();
	// Otherwise decrease blacks time

	drawBackground();
	drawTime();
	drawPieces(board, userColor);

	m_window.display();
}

bool GUIBoard::isOpen() {
	return m_window.isOpen();
}

void GUIBoard::drawBackground() {
	sf::RectangleShape square;
	int offset = 3 * TILE_SIZE;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			square.setPosition(offset + j * TILE_SIZE, i * TILE_SIZE);
			square.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
			// Change this based on whether player is playing black or white
			if ((i + j) % 2) {
				square.setFillColor(sf::Color(161, 111, 90));
			}
			else {
				square.setFillColor(sf::Color(235, 210, 183));
			}
			m_window.draw(square);
		}
	}
}

static const int chessPieceTextureSize = 333;
static int offsetFromPieceType(chess::PieceType type) {
	int index = 0;
	if (type == chess::PieceType::PAWN) {
		index = 5;
	}
	else if (type == chess::PieceType::BISHOP) {
		index = 2;
	}
	else if (type == chess::PieceType::KNIGHT) {
		index = 3;
	}
	else if (type == chess::PieceType::ROOK) {
		index = 4;
	}
	else if (type == chess::PieceType::QUEEN) {
		index = 1;
	}
	else if (type == chess::PieceType::KING) {
		index = 0;
	}
	return index * chessPieceTextureSize;
}

static chess::Color oppose(chess::Color c) {
	if (c == chess::Color::WHITE) {
		return chess::Color::BLACK;
	}
	return chess::Color::WHITE;
}


void GUIBoard::drawPiece(const chess::Board& board, chess::Color userColor, const chess::PieceType type, bool us) {
	chess::Bitboard bits;
	chess::Color pieceColor;

	// The idea is to draw the pieces like before but only use us when we are playing white and use them when we are playing black
	// There is no need to query for whether we are rendering ourselves or the enemy, all we need is the users color(to check whether we should use us or them)
	// and the color of the pieces we are currently drawing(either black or white)

	if (userColor == chess::Color::WHITE) {
		if (us) {
			pieceColor = chess::Color::WHITE;
		}
		else {
			pieceColor = chess::Color::BLACK;
		}
		bits = board.us(pieceColor) & board.pieces(type);
	}
	else {
		if (us) {
			pieceColor = chess::Color::BLACK;
		}
		else {
			pieceColor = chess::Color::WHITE;
		}
		bits = board.them(pieceColor) & board.pieces(type);
	}
	
	for (int i = 0; i < 64; i++) {
		int x = i % 8;
		int y = (int)(i / 8);
		y = 7 - y;
		if ((bits >> i) & 1) {
			// Draw piece depending on type and color
			sf::RectangleShape rect;
			rect.setTexture(&m_pieceTexture);
			rect.setPosition(sf::Vector2f(x * TILE_SIZE + TILE_SIZE * 3, y * TILE_SIZE));
			int tx_offset = offsetFromPieceType(type);
			int ty_offset = 0;
			if (pieceColor == chess::Color::BLACK) {
				ty_offset = chessPieceTextureSize;
			}
			rect.setTextureRect(sf::IntRect(tx_offset, ty_offset, chessPieceTextureSize, chessPieceTextureSize));
			rect.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
			m_window.draw(rect);
		}
	}
}

void GUIBoard::drawPieces(const chess::Board& board, chess::Color userColor) {
	// Draw all pawns
	drawPiece(board, userColor, chess::PieceType::PAWN, true);
	drawPiece(board, userColor, chess::PieceType::KNIGHT, true);
	drawPiece(board, userColor, chess::PieceType::BISHOP, true);
	drawPiece(board, userColor, chess::PieceType::ROOK, true);
	drawPiece(board, userColor, chess::PieceType::QUEEN, true);
	drawPiece(board, userColor, chess::PieceType::KING, true);

	drawPiece(board, userColor, chess::PieceType::PAWN, false);
	drawPiece(board, userColor, chess::PieceType::KNIGHT, false);
	drawPiece(board, userColor, chess::PieceType::BISHOP, false);
	drawPiece(board, userColor, chess::PieceType::ROOK, false);
	drawPiece(board, userColor, chess::PieceType::QUEEN, false);
	drawPiece(board, userColor, chess::PieceType::KING, false);
}

static std::string twoDigit(int n) {
	if (n < 10) {
		return std::string("0") + std::to_string(n);
	}
	return std::to_string(n);
}

static std::string timeToString(float t) {
	int seconds = (int)t % 60;
	int minutes = (int)(t / 60);
	std::string secondsText = twoDigit(seconds);
	std::string minutesText = twoDigit(minutes);
	return minutesText + ":" + secondsText;
}

void GUIBoard::drawPlayerTime(float t, float yPos, bool userMove) {
	sf::Text text;
	text.setFont(m_font);
	text.setString(timeToString(t));
	text.setCharacterSize(192 * TILE_SIZE / 180);

	text.setPosition(TILE_SIZE * 11 + TILE_SIZE * 0.2, yPos);

	text.setFillColor(sf::Color::White);
	m_window.draw(text);
}

void GUIBoard::drawTime() {
	sf::RectangleShape separation;
	separation.setFillColor(sf::Color::White);
	separation.setPosition(TILE_SIZE * 11 + TILE_SIZE * 0.5, TILE_SIZE * 4 - 4);
	separation.setSize(sf::Vector2f(TILE_SIZE * 2, 8));
	m_window.draw(separation);

	drawPlayerTime(m_whiteTime, TILE_SIZE * 2.5, true);
	drawPlayerTime(m_blackTime, TILE_SIZE * 4.15, false);
}