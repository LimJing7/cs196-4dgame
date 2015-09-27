# Sample Maekfile
# Change whatever fields necessary
# __DON'T RUN THIS SCRIPT WITH sudo!!!__
# Detects if running on Darwin or Linux.

CXX=clang++
LD=clang++
WARNINGS=-pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS=-std=c++1y -stdlib=libc++ -g -c
LDFLAGS=-std=c++1y -stdlib=libc++ -lc++abi
OGL_LIBRARY_PATH=$(HOME)/opengl/lib
OGL_INCLUDE_PATH=$(HOME)/opengl/include
OGL_STATIC_LIBS=$(OGL_LIBRARY_PATH)/libGLEW.a
OGL_DYNAMIC_LIBS=-lglfw
OUTFILE=shaders

UNAME_S=$(shell uname -s)
ifeq ($(UNAME_S),Linux)
	OGL_EXTRA_LIBS=-lGL -lX11 -lpthread -lXrandr -lXi
	CXXFLAGS+=-D__LINUXOS__
endif
ifeq ($(UNAME_S),Darwin)
	OGL_EXTRA_LIBS=-framework OpenGL
	CXXFLAGS+=-D__APPLEOS__
endif
	
all: build run

build: main.o
	$(LD) $^ -o $(OUTFILE) -L$(OGL_LIBRARY_PATH) $(LDFLAGS) -Wl,-rpath $(OGL_LIBRARY_PATH) $(OGL_STATIC_LIBS) $(OGL_DYNAMIC_LIBS) $(OGL_EXTRA_LIBS)

main.o: main.cpp shader.h
	$(CXX) $(CXXFLAGS) $(WARNINGS) -I$(OGL_INCLUDE_PATH) $<

.PHONY: run clean

run:
	./$(OUTFILE)

clean:
	rm -rf *.o $(OUTFILE)
