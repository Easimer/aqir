#
# Makefile - Aqir Loader Makefile
# Author: Daniel Meszaros <easimer@gmail.com>
# EasimerNet-Confidental
#

CC=g++
CMFLAGS=-flto
INCLUDES=-I include/
CXXFLAGS=$(CMFLAGS) -ggdb -std=c++1z -Wall -O2 -fPIC -mtune=native -march=native $(INCLUDES)
LDFLAGS=$(CMFLAGS) -shared -Bstatic -ldl -Bstatic -lpthread

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

EXE=aqirl.so

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) $(LDFLAGS)

src/%.o: src/%.cpp include/%.h
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJECTS) $(EXE)

install: $(EXE)
	cp $(EXE) "/home/easimer/.wine/drive_c/Tauri Launcher/MoP/"

