#include <iostream>
#include <thread>
#include "GUIBoard.h"

GUIBoard guiBoard;

void takeInput() {
    while (guiBoard.isOpen()) {
        std::string s;
        std::cin >> s;
        std::cout << s << std::endl;
    }
}

int main()
{
    guiBoard.create();
    std::thread t(takeInput);

    while (guiBoard.isOpen())
    {
        guiBoard.draw();
    }

    t.join();

    return 0;
}