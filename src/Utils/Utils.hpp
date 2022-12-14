#ifndef MATH_ALGEBRA_LINEAR_UTILS_HPP
#define MATH_ALGEBRA_LINEAR_UTILS_HPP

#include <stdexcept>

namespace math::algebra::linear
{
#if ALLOW_NEGATIVE_INDEX == 0
    typedef size_t size_type;
#else
    typedef ssize_t size_type;
#endif
    namespace details
    {
#if ALLOW_NEGATIVE_INDEX == 0
        size_type index_check(size_type index,
                              size_type oor_index)
        {
            if (index >= oor_index || index < 0)
                throw std::runtime_error("Index out of range.");
            return index;
        }
#else
        size_type index_check(size_type index,
                              size_type oor_index)
        {
            if (index >= oor_index || index < -oor_index)
                throw std::runtime_error("Index out of range.");
            return index >= 0 ? index : oor_index + index;
        }
#endif
    } // namespace details
} // namespace math::algebra::linear

#endif // MATH_ALGEBRA_LINEAR_UTILS_HPP