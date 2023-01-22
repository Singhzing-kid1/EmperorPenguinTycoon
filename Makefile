# MAKEFILE

CXX = g++
CFLAGS = -w -g -std=c++14

#libraries

INCLUDE =  -Iusr/local/include
LDFLAGS = -Lusr/local/lib
LDLIBS = -ljsoncpp

# DETAILS

SOURCE = main.cpp
OUT = main

all: build

build: $(SOURCE)
	$(CXX) -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(SOURCE)

clean:
	rm -f *o $(OUT)
