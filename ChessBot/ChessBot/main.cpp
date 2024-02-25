#include <iostream>
#include <thread>
#include "GUIBoard.h"

GUIBoard guiBoard;

static void takeInput() {
    while (guiBoard.isOpen()) {
        std::string s;
        std::cin >> s;
        std::cout << s << std::endl;
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
        guiBoard.draw();
    }

    t.join();

    return 0;
}