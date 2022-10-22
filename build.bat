@echo off

mkdir "build"
g++ -o "build\build.exe" -I"include" "src\glad.c" "src\main.cpp" -lmingw32 -lOpengl32 -L"lib" -lglfw3dll
