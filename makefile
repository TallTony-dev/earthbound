makefile:
COMPILER = clang++
MACOS_OPT = -std=c++23 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL raylib/src/libraylib.a
MACOS_OUT = -o "bin/build_osx"
CFILES = src/*.cpp
WINDOWS_OPT = -std=c++23 raylib/src/libraylib.a -lopengl32 -lgdi32 -lwinmm -lshell32 -luser32 -lkernel32
WINDOWS_OUT = -o "bin/build_windows"

all: build_osx build_windows

build_osx:
	$(COMPILER) $(CFILES) $(MACOS_OUT) $(MACOS_OPT)

build_windows:
	$(COMPILER) $(CFILES) $(WINDOWS_OUT) $(WINDOWS_OPT)

clean:
	rm -f *.o myprogram