#include <iostream>
#include "src/Container.hpp"
#include <tuple>
#include <type_traits>

using namespace container;

int main(int argc, char **argv)
{
    tensor<double, 2, 3, 5> t1{};
    t1(0, 0, 0) = 99;
    tensor<double> t2{t1};

    tensor<double, 2, 3, 5> t3{std::move(t2)};

    std::cout << t2.shape(0) << '\n';

    return 0;
}