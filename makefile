
# Set compiler and flags; we'll use g++ since that seems to go pretty fast here.
CXX = g++
CXXFLAGS = --std=c++20 -I.

# Sources, objects.
sources = main.cpp Structures.cpp Support.cpp Simulation.cpp
objects = $(subst .cpp,.o,$(sources))

all: simulation

simulation: $(objects)
	$(CXX) $(CXXFLAGS) -o simulation $(objects)

simulation.o: main.cpp Structures.h 

Structures.o: Structures.h Structures.cpp Support.h

Support.o: Support.h Support.cpp

clean:
	rm -f *.o
