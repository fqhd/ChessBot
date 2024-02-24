#include "GUIBoard.h"

const int TILE_SIZE = 180;

void GUIBoard::create() {
	sf::VideoMode mode;
	mode.width = TILE_SIZE*14;
	mode.height = TILE_SIZE *8;
	m_window.create(mode, "ChessBot", sf::Style::Default);
}

void GUIBoard::draw() {
	m_window.clear(sf::Color(31, 31, 31));
	sf::Event e;
	m_window.pollEvent(e);
	switch (e.type) {
	case sf::Event::Closed:
		m_window.close();
		break;
	}

	drawBackground();

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