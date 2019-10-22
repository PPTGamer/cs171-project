CXX=g++
GCC_VERSION = $(shell $(CXX) -dumpversion)
GCCFLAGS= -static-libgcc -static-libstdc++ -DSFML_STATIC 
LINKER= -I$(SFMLDIR)/include -L$(SFMLDIR)/lib
LIBRARIES=-lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32  -lwinmm  -lgdi32
EXENAME = Project
DEBUGOUT = compile.out

ifeq ("$(GCC_VERSION)", "8.2.0")
	SFMLDIR=libs/SFML-2.5.1-build-mingw_8.2.0-x86_64
else
	SFMLDIR=libs/SFML-2.5.1-build-msys2_9.2.0-x86-64
endif
.PHONY: clean 
make: clean
	@echo Using SFML libs in $(SFMLDIR).
	@echo Compiling...
	$(CXX) main.cpp -o $(EXENAME).exe $(GCCFLAGS) $(LINKER) $(LIBRARIES) 2>$(DEBUGOUT)
	@if exist $(EXENAME).exe (echo Compilation successful.) else (echo Compilation unsuccessful, refer to $(DEBUGOUT) for errors.)

clean:
	@if exist $(EXENAME).exe (del $(EXENAME).exe && echo Old build deleted.)
