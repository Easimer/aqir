#
# Makefile - Aqir Makefile
# Author: Daniel Meszaros <easimer@gmail.com>
# EasimerNet-Confidental
#

CC=g++
CMFLAGS=-flto -D AQIR_LOADED
INCLUDES=-I include/
CXXFLAGS=$(CMFLAGS) -ggdb -std=c++1z -Wall -O2 -fPIC -mtune=native -march=native $(INCLUDES)
LDFLAGS=$(CMFLAGS) -shared -Bstatic -ldl -Bstatic -lpthread -Bstatic -lX11 -Bstatic -lXtst -Bstatic -lXext

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

EXE=aqir.so
LOADER=aqirl.so

all: $(EXE) $(LOADER)

$(LOADER):
	cd loader && make

$(EXE): $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) $(LDFLAGS)

src/%.o: src/%.cpp include/%.h
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJECTS) $(EXE)

finstall: $(EXE) $(LOADER)
	cp $(EXE) "/home/easimer/.wine/drive_c/Tauri Launcher/MoP/"
	cd loader && make install

install: $(EXE)
	cp $(EXE) "/home/easimer/.wine/drive_c/Tauri Launcher/MoP/"

run:
	LD_PRELOAD=aqir.so
	wineserver -k
	
