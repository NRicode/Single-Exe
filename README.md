# Single Exe
Single Exe is a sophisticated tool that can convert almost anything into a single .exe file with smaller size.  
  
Some examples of things that it can convert into single .exe file:
- Python / NodeJs / PHP project with multiple .py, .js or .php files along with all it's dependencies.  
- Any .exe program along with it's .dll and other files.  
- Any shell script like .bat, .ps1, .vbs, etc.
- Many more.

## Demo (packing .exe with multiple .dll files)
https://github.com/user-attachments/assets/88ca2cb1-1772-42fd-8291-f76ed14f5e64

## Features
- Designed to work with almost anything.
- Automatically clean up files after app is closed.
- Customizable compression level to reduce file size.
- Stream based decompression which can handle big files with fixed memory usage

## How to use
1. Select a folder that contains all your app's files.  
2. Enter a launch command, this launch command should be the full name of the executable along with it's file name extensions e.g. "node.exe main.js", "python.exe main.py" (without the quotation mark). You can also add any other additional parameters.  
For shell scripts, you should specify the host executable in **full path** e.g. "C:\Windows\System32\cmd.exe /c app.bat".
3. Select the compression level (0 = no compression (fastest, file size unchanged), 10 = max compression (slowest, smallest file size)). Note that higher compression level will significantly slow down the app on startup.  
4. Wait until you see the "Done" button. "portable.exe" will be generated

## Installing
Prebuilt binary is available on [release page](https://github.com/NRicode/Single-Exe/releases)

## How to build
You can build the project with the CMakeLists.txt provided but it only supports MinGW compiler. If you are using MSVC, you should compile all of the source files and link against raylib library provided.

## Compatibility
Since it will automatically clean up files after the application is closed, the packed app should not write it's data into it's own directory but rather according to windows convention e.g AppData folder / registry

