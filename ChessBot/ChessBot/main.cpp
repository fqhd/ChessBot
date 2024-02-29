#include <iostream>
#include <thread>
#include <vector>
#include "GUIBoard.h"
#include "Engine.h"

Engine engine;
GUIBoard guiBoard;
chess::Color userColor;

static void takeInput() {
    while (guiBoard.isOpen()) {
        std::cout << "Move: ";
        std::string s;
        std::cin >> s;
        engine.playMove(s);
    }
}

int main()
{
    std::string answer;
    std::cout << "Which side do you want to play?(w/b): ";
    std::cin >> answer;
    if (answer == "w" || answer == "W") {
        userColor = chess::Color::WHITE;
    }
    else {
        userColor = chess::Color::BLACK;
        engine.makeMove();
    }

    if (guiBoard.create()) {
        std::cout << "An error occured creating the board" << std::endl;
        return -1;
    }

    std::thread t(takeInput);

    while (guiBoard.isOpen())
    {
        guiBoard.draw(engine.getBoard(), userColor);
    }

    t.join();

    return 0;
}