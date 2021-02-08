# pathfinding

To compile, in VSCode : needs recent cmake, C++ and Cmake-tools plugins

Ctrl+Shift+P -> Cmake: Build

Add this to .vscode/settings.json :

```
"cmake.debugConfig": {
    "cwd": "${workspaceFolder}"
}
```

Install Mingw64 with x86_64 architecture and all other default values http://mingw-w64.org/doku.php/download/mingw-builds

Add Mingw64 "bin" folder to PATH environment variable

Remove any other path to g++/gcc (reboot if needed)