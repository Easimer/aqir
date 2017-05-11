#
# Makefile - Aqir Makefile
# Author: Daniel Meszaros <easimer@gmail.com>
# EasimerNet-Confidental
#

CC=g++
CMFLAGS=
CXXFLAGS=$(CMFLAGS) -std=c++1z -Wall -O2 -fPIC -I include/
LDFLAGS=$(CMFLAGS) -shared -Bstatic -ldl -Bstatic -lpthread -Bstatic -lX11 -Bstatic -lXtst -Bstatic -lXext

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

EXE=aqir.so

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS) $(LDFLAGS)

src/%.o: src/%.cpp include/%.h
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJECTS) $(EXE)

install: $(EXE)
	cp aqir.so /usr/lib/

run:
	LD_PRELOAD=aqir.so
	wineserver -k
	
