#include "keyhook.hpp"

int main(int argc, char* argv[]) {
	///initialize the logger. It creates a directory named "logs" (if it doesn't already exist) then opens the file for writing 
	Logger::initialize();
	Logger::log("OK!\n\n");//writes to the file and to the screen ok
	//creates a reference variable keyHook of type KeyHook& and initializes it with the instance returned by the getInstance() function of the KeyHookSingleton class.
	KeyHook& keyHook = KeyHookSingleton::getInstance();
	FileLoader& fileLoader = keyHook.getKeystrokeHandler().getFileLoader();
	if (argc == 3) {
		fileLoader.loadFromFiles(argv[1], argv[2]);
	}
	else {
		fileLoader.loadFromFiles();
	}
	keyHook.getKeystrokeHandler().resizeBuffer(fileLoader.getCodeLength());// resize the buffer 
	keyHook.setup_hook();//setting up the keyboard hook using SetWindowsHookEx
	Logger::shutdown();//close the log file
	return 0;
}
