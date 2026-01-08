makefile:
COMPILER = clang++

CFILES := $(wildcard src/*.cpp src/hudstuff/*.cpp)
OBJECTS := $(CFILES:.cpp=.o)

MACOS_OPT = -std=c++23 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL raylib/src/libraylib.a
MACOS_OUT = -o "bin/build_osx"
WINDOWS_OPT = -std=c++23 raylib/src/libraylib.a -lopengl32 -lgdi32 -lwinmm -lshell32 -luser32 -lkernel32
WINDOWS_OUT = -o "bin/build_windows"

all: build_osx build_windows

build_osx: $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(MACOS_OUT) $(MACOS_OPT)

build_windows: $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(WINDOWS_OUT) $(WINDOWS_OPT)


%.o: %.cpp
	$(COMPILER) -c $< -o $@

clean:
	rm -f *.o myprogram