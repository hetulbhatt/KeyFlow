# KeyFlow
KeyFlow is a powerful C++ project that helps users enhance their productivity by creating customized cheat codes and key combinations. KeyFlow can be used to launch programs, open documents, and emulate keystrokes. Using this tool, you can assign a cheat code or a key combination (shortcut) to execute a program or flush some predetermined string to the active window on a Windows machine.

Features
- Customizable cheat codes: Personalize your experience by assigning unique codes like "pizza," "alarm," or "goodnight" to instantly print specific strings or execute specific programs.
- Convenient shortcuts: Easily fill in frequently-used file names or other phrases using intuitive shortcuts.
- Expanded punctuation options: Enhance your typing with access to punctuation marks not available on your keyboard, such as curved quotes (“…”), copyright symbol (©), or currency symbols (₹, €) effortlessly.
- Quick access to programs, documents, and websites: Launch your favorite programs, open documents, or visit websites with simple keystrokes.
- Personalized signatures: Add a signature to your emails, message boards by just a few key strokes.
- Advanced key combinations: Assign complex key combinations like CTRL+ALT+Q, WINDOWS+Z, or any other combination to execute desired actions efficiently.
- Hotword printing: Define key combinations like CTRL+ALT+M to automate the printing of frequently-used phrases or statements in any active window.
- Seamless integration: KeyFlow seamlessly integrates with MS Windows, allowing you to use cheat codes and key combinations across all applications.

Getting Started
To start using KeyFlow, follow these simple steps:
1. Clone the KeyFlow repository to your local machine running Windows.
2. Build the project by using the command `g++ -std=c++17 main.cpp`.
3. Run the executable file to launch the KeyFlow application. Make sure that the config files (`../config/codes.config` and `../config/shortcuts.config`) are located in their respective directories.
4. If the config files are not found in their default locations, you have the option to provide custom paths via command line arguments. For example, use `main.exe <path-to-shortcuts.config> <path-to-codes.config>` to specify the paths.
5. Personalize your cheat codes, key combinations, hotwords, and associated actions using the guide provided below.
   
   For `shortcuts.config`:
   - The left-hand side of the operator represents the desired key combination.
   - Depending on the operator used, the right-hand side can either be a string to be printed in the active window or the path to an executable program.
   - If the operator is `$`, the string on the right indicates the path of the executable to launch when the key combination is pressed.
   - If the operator is `:`, the string on the right will be printed in the active window.
   - Each operation should be written on a single line.
   - Comments can be added to the file, but they should be placed on separate lines.

  
    ```
    # This is a comment.
    # The following string will be flushed to the active window at the cursor position 
    # when the user presses LEFT CONTROL + LEFT ALT + A.
    LCONTROL+LMENU+A :PRINT this entire string!

    # The following program will be launched when the user presses LEFT ALT + A.
    LMENU+A $C:\Program Files\Sublime Text 3\sublime_text.exe
    ```
    
  For `codes.config`:
  - Specify the desired length of your cheats at the beginning of the file.
  - The left-hand side of the operator should contain the cheat code.
  - Depending on the operator used, the right-hand side can be a string to be printed in the active window or the path to an executable program.
  - If the operator is $, the string on the right indicates the path of the executable to launch when the cheat code is triggered.
  - If the operator is :, the string on the right will be printed in the active window.
  - Each operation should be written on a single line.
  - It's possible to include comments in the file, but they should be placed on separate lines.
    
    ```
    # All over cheat codes will be of length 5.
    CODE LENGTH = 5
    # Typing hello, regardless of the status of caps lock, 
    # will print WORLD on the active window
    HELLO :WORLD
    APPLE :Stay away, Doc!
    # Typing GTAV5 will launch GTA5.exe, if present, of course.
    GTAV5 $C:\Games\Grand Theft Auto V\GTA5.exe
    ```
    
These instructions will help you get started with KeyFlow and enable you to customize its functionalities according to your preferences.

  
Contributing
- We welcome contributions from the open-source community to enhance KeyFlow's functionality and user experience.
