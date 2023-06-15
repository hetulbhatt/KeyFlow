#include "../include/keyflow/keyhook.h"

int main(int argc, char* argv[]) {
    std::cout << "OK!" << std::endl;
    KeyHook& keyHook = KeyHookSingleton::getInstance();
    FileLoader& fileLoader = keyHook.getKeystrokeHandler().getFileLoader();
    if (argc == 3) {
        fileLoader.loadFromFiles(argv[1], argv[2]);
    } else {
        fileLoader.loadFromFiles();
    }
    keyHook.getKeystrokeHandler().resizeBuffer(fileLoader.getCodeLength());
    keyHook.setup_hook();

    return 0;
}
