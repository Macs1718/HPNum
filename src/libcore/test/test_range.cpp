# include <iostream>
# include <algorithm>
# include <vector>
# include "core/range.hpp"

int main()
{
    Core::range<10,5,-2> r;
    for ( auto v : r ) std::cout << v << " ";
    std::cout << std::endl;
    Core::range<5,10,2> r0;
    for ( auto v : r0 ) std::cout << v << " ";
    std::cout << std::endl;
    Core::range<5,10> r2;
    for ( auto v : r2 ) std::cout << v << " ";
    std::cout << std::endl;
    Core::range<5,10,-1> r3;
    for ( auto v : r3 ) std::cout << v << " ";
    std::cout << std::endl;
    Core::range<11,20> r4;
    std::vector<long> dest;
    std::set_union(r2.begin(), r2.end(), r4.begin(), r4.end(), std::back_inserter(dest));
    for ( auto v : dest ) std::cout << v << " ";
    std::cout << std::endl;
    return EXIT_SUCCESS;
}