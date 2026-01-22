##some llm usage for this, made sure to understand a bit though
makefile:
COMPILER = clang++
EMCC = em++

CFILES := $(wildcard src/*.cpp src/**/*.cpp)
OBJECTS := $(CFILES:.cpp=.o)
WEB_OBJECTS := $(CFILES:.cpp=.web.o)

MACOS_OPT = -std=c++23 -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL raylib/src/libraylib_desktop.a
MACOS_OUT = -o "bin/build_osx"
WINDOWS_OPT = -std=c++23 raylib/src/libraylib_desktop.a -lopengl32 -lgdi32 -lwinmm -lshell32 -luser32 -lkernel32
WINDOWS_OUT = -o "bin/build_windows"
WEB_OPT = -std=c++20 -DPLATFORM_WEB -sMAX_WEBGL_VERSION=2 -sUSE_GLFW=3 -sALLOW_MEMORY_GROWTH=1 --preload-file resources raylib/src/libraylib_web.a
WEB_OUT = -o "bin/build_web.html"


all: build_osx build_windows build_web

build_osx: $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(MACOS_OUT) $(MACOS_OPT)

build_windows: $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(WINDOWS_OUT) $(WINDOWS_OPT)

build_web: $(WEB_OBJECTS)
	$(EMCC) $(WEB_OBJECTS) $(WEB_OUT) $(WEB_OPT)

%.o: %.cpp
	$(COMPILER) -c $< -o $@

%.web.o: %.cpp
	$(EMCC) -std=c++20 -DPLATFORM_WEB -c $< -o $@

clean:
	rm -rf src/*.o src/**/*.o src/*.web.o src/**/*.web.o bin/*
