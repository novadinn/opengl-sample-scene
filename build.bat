@echo off

mkdir "build"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
pushd "build"
cl /EHsc /Fe"build" /I"..\include" "..\src\glad.c" "..\src\main.cpp" /link /LIBPATH:"..\lib" "glfw3dll.lib" "assimp.lib"
popd
:: g++ -o "build\build.exe" -I"include" "src\glad.c" "src\main.cpp" -lmingw32 -lOpengl32 -L"lib" -lglfw3dll
