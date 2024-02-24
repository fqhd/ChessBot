#pragma once
#include <SFML/Graphics.hpp>
#include <chrono>

class GUIBoard
{
public:

	int create();
	void draw();
	bool isOpen();

private:

	void drawBackground();
	void drawTime();
	void drawPlayerTime(float t, float yPos, bool userMove);

	sf::Clock m_clock;
	sf::Font m_font;
	float m_whiteTime = 0.0f;
	float m_blackTime = 0.0f;
	sf::RenderWindow m_window;
};

