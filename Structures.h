
#ifndef STRUCTURES
#define STRUCTURES

#include <vector>
#include "Support.h"

using V = std::vector<std::vector<int>>;

// Binary and arbitrary Cartesian products.
std::vector<V> distributiveCartesianProduct(V& A, V& B);
std::vector<V> arbitraryCartesianProduct(std::vector<V>& basis);

// Elementwise sum over an arbitrary number of vectors.
std::vector<int> elementwiseSum(V& summands);

class Simplex {
    /*
    Encodes a simplex in d-space; for example, a 0-simplex is a vertex, a 1-simplex
    is an edge, etc.
    */
    public:
        int dimension;
        int index;
        int spin;
        int orientation;

        std::vector<int> coordinates;
        std::vector<Simplex> simplices; 


        // 0-simplex constructor.
        Simplex(
            const std::vector<int>& coordinates,
            int spin=1,
            int index=0
        );

        // 1- and higher-dimensional simplex constructor.
        Simplex(
            const std::vector<Simplex>& simplices,
            int dimension,
            int orientation=1,
            int spin=1,
            int index=0
        );

        std::string toString();
};

class Lattice {
    /*
    Encodes a d-dimensional lattice.
    */
    public:
        int dimension;                  // Dimension of the lattice.
        std::vector<Simplex> vertices;  // Underlying (dimension)-dimensional matrix of simplices.

        Lattice(const std::vector<int>& corners);

        std::string toString();
};

#endif
