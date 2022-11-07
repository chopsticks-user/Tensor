#ifndef MATH_ALGEBRA_LINEAR_MATRIX_HPP
#define MATH_ALGEBRA_LINEAR_MATRIX_HPP

#include "../Config.hpp"
#include "../Utils/Utils.hpp"

#include <vector>
#include <algorithm>

namespace math::algebra::linear
{
    namespace details
    {
        struct matrix_base
        {
        };
    }

    template <typename data_type, size_type n_rows = 0, size_type n_cols = 0>
    class matrix : public details::matrix_base
    {
        using is_dynamic_matrix = std::is_same<matrix, matrix<data_type, 0, 0>>;

    public:
        matrix() { _initialize_n_elements(n_rows, n_cols); }

        explicit matrix(size_type n_init_rows, size_type n_init_cols = 0)
        {
            _initialize_n_elements(n_init_rows, n_init_cols);
        }

        size_type size() const noexcept { return _data.size(); }

        size_type column_size() const noexcept { return _n_cols; }

        size_type row_size() const noexcept { return _data.size() / _n_cols; }

        auto begin() noexcept { return _data.begin(); }

        auto begin(size_type row_index)
        {
            row_index = details::index_check(row_index, column_size());
            return _data.begin() + row_index * _n_cols;
        }

        auto end() noexcept { return _data.end(); }

        auto end(size_type row_index)
        {
            row_index = details::index_check(row_index, column_size());
            return _data.begin() + (row_index + 1) * _n_cols;
        }

        data_type &operator()(size_type row_index, size_type col_index)
        {
#if ALLOW_INDEX_CHECKING == 1
            row_index = details::index_check(row_index, row_size());
            col_index = details::index_check(col_index, _n_cols);
#endif
            return _data[row_index * _n_cols + col_index];
        }

    private:
        std::vector<data_type> _data;
        size_type _n_cols;

        constexpr void _initialize_n_elements(size_type n_init_rows,
                                              size_type n_init_cols)
        {
            if (n_init_rows < 0 || n_init_cols < 0)
                throw std::runtime_error("Dim < 0");

            if (is_dynamic_matrix::value == true)
                return;

            if (n_init_rows == 0)
                throw std::runtime_error("n_rows == 0 && n_cols > 0");

            _n_cols = n_init_cols > 0 ? n_init_cols : n_init_rows;
            _data.resize(n_init_rows * _n_cols);
        }
    };

} // namespace math::algebra::linear

#endif // MATH_ALGEBRA_LINEAR_MATRIX_HPP