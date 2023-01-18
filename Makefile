# MAKEFILE

CXX = g++
CFLAGS = -w -g 

# DETAILS

SOURCE = main.cpp
OUT = main

all: build

build: $(SOURCE)
	$(CXX) -o $(OUT) $(CFLAGS) $(SOURCE)
