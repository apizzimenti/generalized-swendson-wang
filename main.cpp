
#include "Structures.h"
#include "Support.h"
#include <iostream>

using namespace std;

int main() {
    Lattice L(vector<int> {2, 2, 2});
    std::cout << L.toString();
    return 0;
}
