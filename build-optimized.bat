@echo off
if exist Project.exe (del Project.exe && echo Old build deleted.)
echo Compiling . . .
g++ ^
  main.cpp ^
  -o Project -static-libgcc -static-libstdc++ ^
  -ISFML-2.5.1-build-mingw8.3.0-x86_64/include -LSFML-2.5.1-build-mingw8.3.0-x86_64/lib -DSFML_STATIC ^
  -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s ^
  -lopengl32  -lwinmm  -lgdi32^
  -O4 2>compile.out
if exist Project.exe (echo Compilation successful.) else (echo Compilation unsuccessful, refer to compile.out for errors.)
