# Sample Makefile
# Change whatever fields necessary
# __DON'T RUN THIS SCRIPT WITH sudo!!!__

CXX=clang++
LD=clang++
WARNINGS=-pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS=-std=c++1y -stdlib=libc++ -g -c
LDFLAGS=-std=c++1y -stdlib=libc++ -lc++abi
OGL_LIBRARY_PATH=$(HOME)/opengl/lib
OGL_INCLUDE_PATH=$(HOME)/opengl/include
OGL_STATIC_LIBS=-lGLEW -lsoil2
OGL_DYNAMIC_LIBS=-lglfw
# Unclear if this works or is necessary on OS X
OGL_EXTRA_LIBS=-lGL -lX11 -lpthread -lXrandr -lXi

OUTFILE=3d_slices

all: build run

build: 3d_slices.o shader.o lin_interpolation.o comvec3.o
	$(LD) $^ -o $(OUTFILE) -L$(OGL_LIBRARY_PATH) $(LDFLAGS) -Wl,-rpath $(OGL_LIBRARY_PATH) -Wl,-Bstatic $(OGL_STATIC_LIBS) -Wl,-Bdynamic $(OGL_DYNAMIC_LIBS) $(OGL_EXTRA_LIBS)

3d_slices.o: 3d_slices.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) -I$(OGL_INCLUDE_PATH) $^

shader.o: shader.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) -I$(OGL_INCLUDE_PATH) $^

lin_interpolation.o: lin_interpolation.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) -I$(OGL_INCLUDE_PATH) $^

comvec3.o: comvec3.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGS) -I$(OGL_INCLUDE_PATH) $^


.PHONY: run clean

run:
	./$(OUTFILE)

clean:
	rm -rf *.o $(OUTFILE)
