
# Set compiler and flags; we'll use g++ since that seems to go pretty fast here.
CXX = g++
CXXFLAGS = --std=c++17

# Sources, objects.
sources = main.cpp Structures.cpp Support.cpp # Simulation.cpp
objects = $(subst .cpp,.o,$(sources))

all: main

main: $(objects)
	$(CXX) $(CXXFLAGS) -o main $(objects)

main.o: main.cpp Structures.h 

Structures.o: Structures.h Structures.cpp Support.h

Support.o: Support.h Support.cpp

clean:
	rm -f *.o
