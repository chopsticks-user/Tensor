#include <iostream>
#include "Tensor.hpp"
#include <tuple>
#include <type_traits>

template <typename T, ssize_t... dims>
using tensor = container::details::tensor_base_static<T, dims...>;

int main(int argc, char **argv)
{

    tensor<int, 2, 3, 4, 5> t{};

    t._data[11] = 1111;

    std::cout << t(-2, 0, 2, 1) << '\n';

    return 0;
}