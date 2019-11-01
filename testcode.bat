g++ -static-libgcc -static-libstdc++ -DSFML_STATIC ^
-Ilibs/SFML-2.5.1-build-msys2_9.2.0-x86-64/include ^
-Llibs/SFML-2.5.1-build-msys2_9.2.0-x86-64/lib ^
-lsfml-graphics-s ^
driver.cpp ./ai/* ^
-o Test.exe 2>driver.out
