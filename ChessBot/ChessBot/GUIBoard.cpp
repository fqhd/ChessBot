#include "GUIBoard.h"
#include <iostream>

const int TILE_SIZE = 128;

int GUIBoard::create(bool invert) {
	m_invert = invert;
	sf::VideoMode mode;
	mode.width = TILE_SIZE * 11;
	mode.height = TILE_SIZE * 8;
	
	m_window.create(mode, "ChessBot", sf::Style::Default);
	m_window.setVerticalSyncEnabled(true);
	m_whiteTime = 300.0f;
	m_blackTime = 300.0f;
	if (!m_font.loadFromFile("Roboto-Regular.ttf")) {
		return -1;
	}
	if (!m_pieceTexture.loadFromFile("pieces.png")) {
		return -1;
	}
	m_clock.restart();
	m_pieceTexture.setSmooth(true);
	m_isOpen = true;
	return 0;
}

void GUIBoard::draw(const chess::Board& board) {
	m_window.clear(sf::Color(31, 31, 31));
	sf::Event e;
	m_window.pollEvent(e);
	switch (e.type) {
	case sf::Event::Closed:
		m_isOpen = false;
		m_window.close();
		break;
	}

	if (board.sideToMove() == chess::Color::WHITE) {
		m_whiteTime -= m_clock.getElapsedTime().asSeconds();
	}
	else {
		m_blackTime -= m_clock.getElapsedTime().asSeconds();
	}
	m_clock.restart();

	drawBackground();
	drawTime(board);
	drawPieces(board);

	m_window.display();
}

bool GUIBoard::isOpen() const {
	return m_isOpen;
}

void GUIBoard::drawBackground() {
	sf::RectangleShape square;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			square.setPosition(j * TILE_SIZE, i * TILE_SIZE);
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

void GUIBoard::drawPiece(const chess::Board& board, chess::Color pieceColor, const chess::PieceType type) {
	chess::Bitboard bits = board.us(pieceColor) & board.pieces(type);

	for (int i = 0; i < 64; i++) {
		int x = i % 8;
		int y = (int)(i / 8);
		if (m_invert) {
			y = 7 - y;
		}
		if ((bits >> i) & 1) {
			// Draw piece depending on type and color
			sf::RectangleShape rect;
			rect.setTexture(&m_pieceTexture);
			rect.setPosition(sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE));
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

void GUIBoard::drawPieces(const chess::Board& board) {
	// Draw all pawns
	drawPiece(board, chess::Color::WHITE, chess::PieceType::PAWN);
	drawPiece(board, chess::Color::WHITE, chess::PieceType::KNIGHT);
	drawPiece(board, chess::Color::WHITE, chess::PieceType::BISHOP);
	drawPiece(board, chess::Color::WHITE, chess::PieceType::ROOK);
	drawPiece(board, chess::Color::WHITE, chess::PieceType::QUEEN);
	drawPiece(board, chess::Color::WHITE, chess::PieceType::KING);

	drawPiece(board, chess::Color::BLACK, chess::PieceType::PAWN);
	drawPiece(board, chess::Color::BLACK, chess::PieceType::KNIGHT);
	drawPiece(board, chess::Color::BLACK, chess::PieceType::BISHOP);
	drawPiece(board, chess::Color::BLACK, chess::PieceType::ROOK);
	drawPiece(board, chess::Color::BLACK, chess::PieceType::QUEEN);
	drawPiece(board, chess::Color::BLACK, chess::PieceType::KING);
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

void GUIBoard::drawPlayerTime(float t, float yPos, sf::Color color) {
	sf::Text text;
	text.setFont(m_font);
	text.setString(timeToString(t));
	text.setCharacterSize(192 * TILE_SIZE / 180);

	text.setPosition(TILE_SIZE * 8 + TILE_SIZE * 0.2, yPos);

	text.setFillColor(color);
	m_window.draw(text);
}

void GUIBoard::drawTime(const chess::Board& board) {
	sf::RectangleShape separation;
	separation.setFillColor(sf::Color::White);
	separation.setPosition(TILE_SIZE * 8 + TILE_SIZE * 0.5, TILE_SIZE * 4 - 4);
	separation.setSize(sf::Vector2f(TILE_SIZE * 2, 8));
	m_window.draw(separation);

	if (board.sideToMove() == chess::Color::WHITE) {
		if (m_invert) {
			drawPlayerTime(m_blackTime, TILE_SIZE * 2.5, sf::Color(180, 180, 180));
			drawPlayerTime(m_whiteTime, TILE_SIZE * 4.15, sf::Color::White);
		}
		else {
			drawPlayerTime(m_whiteTime, TILE_SIZE * 2.5, sf::Color::White);
			drawPlayerTime(m_blackTime, TILE_SIZE * 4.15, sf::Color(180, 180, 180));
		}
	}
	else {
		if (m_invert) {
			drawPlayerTime(m_blackTime, TILE_SIZE * 2.5, sf::Color::White);
			drawPlayerTime(m_whiteTime, TILE_SIZE * 4.15, sf::Color(180, 180, 180));
		}
		else {
			drawPlayerTime(m_whiteTime, TILE_SIZE * 2.5, sf::Color(180, 180, 180));
			drawPlayerTime(m_blackTime, TILE_SIZE * 4.15, sf::Color::White);
		}
	}
}