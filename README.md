# KeyFlow
KeyFlow is a powerful C++ project that helps users enhance their productivity by creating customized cheat codes and key combinations. Using this tool, you can assign a cheat code or a key combination (shortcut) to execute a program or flush some predetermined string to the active window on a Windows machine.

Features
- Create custom cheat codes: Assign personalized codes such as "pizza," "alarm," or "goodnight" to print specific strings or execute specific programs instantly.
- Key combinations: Assign complex key combinations like CTRL+ALT+Q, WINDOWS+Z, or any other combination to execute a desired action.
- Hotword printing: Set up key combinations like CTRL+ALT+7 to print a predefined phrase, such as "I love CR7," on any active window.
- Seamless integration: KeyMapper works seamlessly with MS Windows OS, allowing you to utilize cheat codes and key combinations across all applications.

Getting Started
To get started with KeyMapper, follow these steps:

- Clone the KeyMapper repository to your local machine running Windows.
- Build the project using g++ main.cpp.
- Run the executable file to launch the KeyMapper application. Ensure that the config files (../config/codes.config and ../config/shortcuts.config) are present at the appropriate locations.
- If the config files are not found, you can provide a custom path to the config files via command line arguments. For example, main.exe <path-to-shortcuts.config> <path-to-codes.config>.
- Customize your cheat codes, key combinations, hotwords, and actions using the guide provided below.

  shortcuts.config:
  - The left-hand side of the operator should contain the key combination.
  - Depending on the operator used, the right-hand side can either be a string to be flushed on the active window or a program to be executed.
  - If the operator is $, the string to the right indicates the path of the executable to be launched when this key combination is pressed.
  - If the operator is :, the string to the right will be flushed to the active window.
  - Each operation should not span more than one line.
  - You can add comments to the file, but they should be on separate lines.
  ```
  # This is a comment.
  # The following string will be flushed to the active window at the cursor position when the user presses LEFT CONTROL + LEFT ALT + A.
  LCONTROL+LMENU+A :PRINT this entire string!

  # The following program will be launched when the user presses LEFT ALT + A.
  LMENU+A $C:\Program Files\Sublime Text 3\sublime_text.exe
  ```
Contributing
We welcome contributions from the open-source community to enhance KeyFlow's functionality and user experience.
