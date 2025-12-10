makefile:
COMPILER = clang
MACOS_OPT =  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL raylib/src/libraylib.a
MACOS_OUT = -o "bin/build_osx"
CFILES = src/*.c

all: build_osx
build_osx:
	$(COMPILER) $(CFILES) $(MACOS_OUT) $(MACOS_OPT)

clean:
	rm -f *.o myprogram