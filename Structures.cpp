
#include <iostream>
#include <numeric>
#include <vector>
#include <linbox/matrix/sparse-matrix.h>

#include "Structures.h"
#include "Support.h"

using namespace std;

// Binary Cartesian product: pairs up all the vectors in A and B.
std::vector<V> distributiveCartesianProduct(V& A, V& B) {
    std::vector<V> product;

    for (const auto& b : B) {
        V concat = A;
        concat.push_back(b);
        product.push_back(concat);
    }

    return product;
}

// This implementation of the Cartesian product takes in some "basis" (i.e. all
// integer multiples of basis vectors for each axis on the d-dimensional integer
// lattice) and dynamically finds all the combinations of vectors; this generates
// all coordinates in the lattice.
std::vector<V> arbitraryCartesianProduct(std::vector<V>& basis) {
    // `product` will be dynamically resized as necessary.
    std::vector<V> product;

    // We'll attack this in a dynamic programming way: first, find the product(s)
    // of the first two elements, then the products of all the results with the
    // next element, and so on.

    // Initialize `product` to be singletons, each containing an element of
    // the first set of vectors of `basis`. For example, if `basis` is
    //
    //                  { {a, b, c}, {p, q, r}, {x, y, z} },
    //
    // then `product` will be initialized to
    //
    //                          { {a}, {b}, {c} }
    //
    // so at the next step it becomes
    //
    //                    { {a, p}, {a, q}, ... , {c, r} }.
    //
    // We continue this procedure until we've computed the products of *all*
    // vectors, then hand things off to the `Lattice` constructor to finish
    // computing coordinates for 0-simplices. Another way to think about these
    // vectors of vectors is as linear combinations, where each vector in the
    // combination is pairwise linearly independent from the others; this way,
    // we're simply finding all possible (integer) linear combinations of the
    // basis vectors.
    for (const auto& initial : basis.at(0)) {
        V _initial {initial};
        product.push_back(_initial);
    }

    for (int focus=1; focus < basis.size(); focus++) {
        std::vector<V> intermediate;
        for (auto& subproduct : product) {
            // Compute the binary Cartesian product of the particular subproduct
            // (some list of vectors) and the current list of vectors we're
            // focusing on (from the basis). Then, add the concatenated vectors
            // to our intermediate product list.
            std::vector<V> sub = distributiveCartesianProduct(subproduct, basis.at(focus));
            std::copy(sub.begin(), sub.end(), std::back_inserter(intermediate));
        }

        // Overwrite `product`. At the last step, this should contain vectors
        // of vectors to be elementwise summed.
        // product = intermediate;
        product = intermediate;
    }
    
    return product;
}

// Computes the elementwise some of some number of vectors, all assumed to have
// the same dimensions.
std::vector<int> elementwiseSum(V& summands) {
    // Vector whose ith position will be the sum of the entries of the ith
    // positions of all vectors in `summands`.
    std::vector<int> sum;

    // First iterate over the rows, creating subsums as we go; for each row, access
    // all the values at the `row`th entry of each vector in `summands`, then push
    // this value to `sum`.
    for (int row=0; row < summands.at(0).size(); row++) {
        int subsum = 0;
        for (int index=0; index < summands.size(); index++) {
            subsum += summands.at(index).at(row);
        }
        sum.push_back(subsum);
    }
    
    return sum;
}

// Implementation of the 0-simplex constructor.
Simplex::Simplex(
    const std::vector<int>& coordinates,
    int spin,
    int index
) {
    // Set the dimension to 0; these are vertices in our integer lattice.
    this->dimension = 0;
    this->coordinates = coordinates;
    this->index = index;
    this->spin = spin;
}

// Implementation of the i-simplex constructor for i > 0.
Simplex::Simplex(
    const std::vector<Simplex>& simplices,
    int dimension,
    int orientation,
    int spin,
    int index
) {
    // Record the dimension of the simplex.
    this->dimension = dimension;

    // We can go through this vector of simplices and access the spins of each
    // (i-1)-dimensional simplex.
    this->simplices = simplices;

    // In which direction do we traverse the (i-1)-simplices?
    this->orientation = orientation;
    this->spin = spin;
}

std::string Simplex::toString()  {
    return Support::stringifyOne(this->coordinates);
}


Lattice::Lattice(const vector<int>& corners) {
    // First, we'll create a vector of vectors of vectors: that is, for each
    // dimension, we'll create "axes," from which we'll derive our coordinates.
    std::vector<V> basis;

    for (int axis=0; axis < corners.size(); axis++) {
        // Initialize a vector with all zeros; we'll replace the appropriate
        // zeros with scaled values on an axis. Note that we include the zero
        // vector *on each axis*, otherwise we wouldn't be able to access the
        // "edges" of our sublattice.
        V basisForAxis;
        for (int k=0; k < corners.at(axis)+1; k++) {
            vector<int> e(corners.size());
            e.at(axis) = k;
            basisForAxis.push_back(e);
        }
        basis.push_back(basisForAxis);
    }

    // Now that we've constructed our basis, we want to get *all* the linear
    // combinations, then add those fellas up!
    std::vector<V> combinations = arbitraryCartesianProduct(basis);
    V coordinates;

    for (auto& combination : combinations) {
        std::vector<int> sum = elementwiseSum(combination);
        coordinates.push_back(sum);
    }

    // // Even though they're in a reasonably desirable order, we want to sort them
    // // in a dictionary order so everything's topologically easier; we can then
    // // create a matrix using the following strategy: given "corners" (c1, ..., ck),
    // // we can put the first c1 vectors into one row, the next c2 vectors in the
    // // next row, and so on.
    // coordinates = Support::sortMany(coordinates);

    // Now, we can create 0-simplices for each of the coordinates.
    for (auto& coordinate : coordinates) {
        this->vertices.push_back(Simplex(coordinate));
    }
}

std::string Lattice::toString() {
    std::string object;

    object += std::to_string(this->dimension) + "-dimenisional integer lattice on coordinates\n\n";
    object += "{\n";

    for (auto& simplex : this->vertices) {
        object += "\t" + simplex.toString() + "\n";
    }

    object += "}\n";

    return object;
}
