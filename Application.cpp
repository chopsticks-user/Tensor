#include <iostream>
#include "src/Container.hpp"
#include <tuple>
#include <type_traits>

using namespace container;

int main(int argc, char **argv)
{
    tensor<double> t{};

    std::cout << t.order() << '\n';

    return 0;
}