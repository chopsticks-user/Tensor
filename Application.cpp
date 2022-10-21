#include <iostream>
#include "src/Container.hpp"
#include <tuple>
#include <type_traits>

using namespace container;

int main(int argc, char **argv)
{
    tensor<double> t{3, 3, 3, 3, 3, 3};

    std::cout << t.size() << '\n';

    return 0;
}