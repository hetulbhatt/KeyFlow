#include "keyhook.hpp"
#include "Window.hpp"
#include <future>

int main(int argc, char* argv[]) {
	// Initialize the logger. It creates a directory named "logs" (if it doesn't already exist) then opens the file for writing 
	Logger::initialize();
	Logger::log("OK!\n\n");// Writes to the file and to the screen ok
	// Creates a reference variable keyHook of type KeyHook& and initializes it with the instance returned by the getInstance() function of the KeyHookSingleton class.
	KeyHook& keyHook = KeyHookSingleton::getInstance();
	Window& window = Window::GetWindowSingleton();
	FileLoader& fileLoader = keyHook.getKeystrokeHandler().getFileLoader();
	if (argc == 3) {
		fileLoader.loadFromFiles(argv[1], argv[2]);
	}
	else {
		fileLoader.loadFromFiles();
	}
	keyHook.getKeystrokeHandler().resizeBuffer(fileLoader.getCodeLength());// Resize the buffer 
	auto i = std::async(std::launch::async, &keyHook.setup_hook);// Setting up the keyboard hook using SetWindowsHookEx
	window.Run();
	
	Logger::shutdown();// Close the log file
	return 0;
}
