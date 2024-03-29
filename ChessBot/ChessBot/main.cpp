#include <iostream>
#include <thread>
#include <vector>
#include "GUIBoard.h"
#include "Engine.h"

Engine engine;
GUIBoard guiBoard;

static int executeCommand(const std::string& command) {
    if (command == "B") {
        // in progress, supposed to be move takeback
        return -1;
    }

    return engine.playMove(command);
}

static void takeInput() {
    while (guiBoard.isOpen()) {
        if (guiBoard.sentCommand()) {
            if (executeCommand(guiBoard.getCommand())) {
                guiBoard.commandFailed();
            }
            else {
                guiBoard.commandExecuted();
                std::chrono::duration<float> dur(0.1);
                std::this_thread::sleep_for(dur);
                engine.makeMove();
            }
        }
        std::chrono::duration<float> dur(0.01);
        std::this_thread::sleep_for(dur);
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
        guiBoard.update();
        guiBoard.draw(engine.getBoard());
    }

    t.join();

    return 0;
}