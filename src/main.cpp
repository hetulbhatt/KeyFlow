#include "../include/keyflow/keyhook.h"

int main() {
    std::cout << "OK!" << std::endl;
    KeyHook& keyHook = KeyHookSingleton::getInstance();
    FileLoader& fileLoader = keyHook.getKeystrokeHandler().getFileLoader();
    fileLoader.load_codes();
    fileLoader.load_shortcuts();
    keyHook.getKeystrokeHandler().resizeBuffer(fileLoader.getCodeLength());
    keyHook.setup_hook();

    return 0;
}
