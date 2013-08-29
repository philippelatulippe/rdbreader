
CXX = g++
CC = gcc

CPPFLAGS = -Wall -g -std=c++0x 

LFLAGS = -std=c++0x -lstdc++

OUT = rdbreader fctxconverter lipreader

SRC_rdbreader = 	rdbreader.cpp Buffer.cpp
SRC_fctxconverter = 	fctxconverter.cpp
SRC_lipreader = 	lipreader.cpp

#This Makefile is terrible! Who coded this?

.PHONY: depend clean

all:    $(OUT)

rdbreader:
	$(CXX) $(CPPFLAGS) $(INCLUDES) -o rdbreader  $(SRC_rdbreader) $(LFLAGS) $(LIBS)

fctxconverter:
	$(CXX) $(CPPFLAGS) $(INCLUDES) -o fctxconverter  $(SRC_fctxconverter) $(LFLAGS) $(LIBS)


lipreader:
	$(CXX) $(CPPFLAGS) $(INCLUDES) -o lipreader  $(SRC_lipreader) $(LFLAGS) $(LIBS)

clean:
	rm $(OUT)
