#include "../include/keyflow/keyhook.hpp"
#include <stdio.h>
#include <string_view>
#include <iostream>
#include "../include/keyflow/systemtraymanager.hpp"  

//using namespace std::literals;
int main(int argc, char* argv[]) {
    Logger::initialize();
    Logger::log("OK!\n\n");
    SystemTrayManager tray{GetConsoleWindow()};
    KeyHook& keyHook = KeyHookSingleton::getInstance();
    FileLoader& fileLoader = keyHook.getKeystrokeHandler().getFileLoader();
    if (argc == 3) {
        fileLoader.loadFromFiles(argv[1], argv[2]);
    } else {
        fileLoader.loadFromFiles();
    }
    keyHook.getKeystrokeHandler().resizeBuffer(fileLoader.getCodeLength());

    keyHook.setup_hook();
    Logger::shutdown();
    return 0;
}
