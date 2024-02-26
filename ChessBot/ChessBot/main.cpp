#include <iostream>
#include <thread>
#include "GUIBoard.h"
#include "Engine.h"

Engine engine;
GUIBoard guiBoard;

static void takeInput() {
    while (guiBoard.isOpen()) {
        std::string s;
        std::cin >> s;
        engine.playMove(s);
    }
}

int main()
{
    if (guiBoard.create()) {
        std::cout << "An error occured creating the board" << std::endl;
        return -1;
    }

    std::thread t(takeInput);

    while (guiBoard.isOpen())
    {
        guiBoard.draw(engine.getBoard());
    }

    t.join();

    return 0;
}