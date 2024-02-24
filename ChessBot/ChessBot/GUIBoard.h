#pragma once
#include <SFML/Graphics.hpp>

class GUIBoard
{
public:

	void create();
	void draw();
	bool isOpen();

private:

	void drawBackground();

	sf::RenderWindow m_window;
};

