
#include <iostream>
#include <vector>
#include "Support.h"

using namespace std;

// Turn a single vector into a string.
std::string Support::stringifyOne(const std::vector<int>& vector) {
    std::string object;

    object += "{";
    for (int i=0; i < vector.size(); i++) {
        object += std::to_string(vector.at(i));
        object += (i < vector.size()-1) ? ", " : "";
    }
    object += "}";

    return object;
}

// Print a single vector in a nice fasion; the user can print a new line afterwards
// if they so desire.
void Support::printOne(const std::vector<int>& vector, bool trailer) {
    std::cout << "{";
    for (int i=0; i < vector.size(); i++) {
        std::cout << vector.at(i) << ((i < vector.size()-1) ? ", " : "");
    }
    std::cout << "}" << (trailer ? "\n" : "");
}

// Print a bunch of vectors (a vector of vectors) utilizing `printOne`.
void Support::printMany(const V& vectors) {
    std::cout << "{" << std::endl;
    for (int i=0; i < vectors.size(); i++) {
        std::cout << "\t";
        printOne(vectors.at(i), false);
        std::cout << ((i < vectors.size()-1) ? "," : "") << std::endl;
    }
    std::cout << "}" << std::endl;
}

// Compare two vectors.
static bool invertedDictionaryOrder(const std::vector<int>& u, const std::vector<int>& v) {
    // Assert vectors are the same size before we do anything.
    assert(u.size() == v.size());

    // Now we want to do a dictionary order backwards: that is, the "smallest"
    // nonzero vector is (1,0,0) and the "largest" nonzero vector is (0,0,1).
    for (int i=u.size(); i >= 0; i--) {
        if (u[i] < v[i]) return true;
    }
    return false;
}

// Sort a vector of integer vectors.
V Support::sortMany(const V& vectors) {
    V sortable = vectors;
    std::sort(sortable.begin(), sortable.end(), invertedDictionaryOrder);
    return sortable;
}
