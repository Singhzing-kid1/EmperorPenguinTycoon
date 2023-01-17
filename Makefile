# MAKEFILE

CXX = g++
CFLAGS = -w -g 

# LIBRARIES

INCLUDE = -Inix/store/9ics1jllagy3llvdbqckfh6gsrk0vgis-curlpp-0.8.1/include -Inix/store/a3gs2ca74npr332wvqqvl2p7m57x4ksx-curl-7.76.1-dev/include
LDFLAGS = -Lnix/store/9ics1jllagy3llvdbqckfh6gsrk0vgis-curlpp-0.8.1/lib  -Lnix/store/9m7zlxss40ppi1qsyadzkj13drhzc8hz-curl-7.76.1/lib
LDLIBS = -lcurl -lcurlpp

# DETAILS

SOURCE = main.cpp
OUT = main

all: build

build: $(SOURCE)
	$(CXX) -o $(OUT) $(INCLUDE) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(SOURCE)
