#include <iostream>
#include <thread>
#include <vector>
#include "GUIBoard.h"
#include "Engine.h"

Engine engine;
GUIBoard guiBoard;

static void takeInput() {
    while (guiBoard.isOpen()) {
        
        engine.playMove();
        engine.makeMove();
    }
}

int main()
{
    bool invert = true;
    std::string answer;
    std::cout << "Which side do you want to play?(w/b): ";
    std::cin >> answer;
    if (answer == "b" || answer == "B") {
        invert = false;
    }

    if (guiBoard.create(invert)) {
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