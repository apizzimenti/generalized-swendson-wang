
#ifndef SUPPORT
#define SUPPORT

#include <vector>

using V = std::vector<std::vector<int>>;

namespace Support {
    void printOne(const std::vector<int>& vector, bool trailer=true);
    void printMany(const V& vectors);
    
    std::string stringifyOne(const std::vector<int>& vector);
    
    V sortMany(const V& vectors);
}

#endif
