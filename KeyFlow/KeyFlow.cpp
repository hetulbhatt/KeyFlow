#include "keyhook.hpp"

int main(int argc, char* argv[]) {
    Logger::initialize();
    Logger::log("OK!\n\n");
    KeyHook& keyHook = KeyHookSingleton::getInstance();
    FileLoader& fileLoader = keyHook.getKeystrokeHandler().getFileLoader();
    if (argc == 3) {
        fileLoader.loadFromFiles(argv[1], argv[2]);
    }
    else {
        fileLoader.loadFromFiles();
    }
    keyHook.getKeystrokeHandler().resizeBuffer(fileLoader.getCodeLength());
    keyHook.setup_hook();
    Logger::shutdown();
    return 0;
}
