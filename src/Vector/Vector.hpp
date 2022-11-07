#ifndef MATH_ALGEBRA_LINEAR_VECTOR_HPP
#define MATH_ALGEBRA_LINEAR_VECTOR_HPP

#include "../Config.hpp"
#include "../Utils/Utils.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>

namespace math::algebra::linear
{
    namespace details
    {
        struct vector_base
        {
        };
    }

    template <typename data_type, size_type n_cols = 0, size_type n_rows = 0>
    class vector : public details::vector_base
    {
    public:
    private:
    };

} // namespace math::algebra::linear

#endif // MATH_ALGEBRA_LINEAR_VECTOR_HPP