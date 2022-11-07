#ifndef MATH_ALGEBRA_LINEAR_TENSOR_HPP
#define MATH_ALGEBRA_LINEAR_TENSOR_HPP

#include "../Config.hpp"
#include "../Utils/Utils.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>

namespace math::algebra::linear
{
    // template <typename data_type, size_type... dims, allocator>
    template <typename data_type, size_type... dims>
    class tensor
    {
        typedef tensor<data_type> dynamic_tensor;
        using is_dynamic_tensor = std::is_same<tensor, dynamic_tensor>;

    public:
        tensor() : _dims{dims...}
        {
            if constexpr (is_dynamic_tensor::value == false)
                this->_initialize();
        }

        template <typename... T>
        explicit tensor(T... init_dims) : _dims{init_dims...}
        {
            static_assert(is_dynamic_tensor::value == true,
                          "List initialization is for dynamic tensors only.");
            this->_initialize();
        }

        tensor(const tensor &rhs_tensor) = default;

        template <size_type... rhs_dims>
        tensor(const tensor<data_type, rhs_dims...> &rhs_tensor) : _dims{dims...}
        {
            this->_diff_type_copy_initialize(rhs_tensor);
        }

        tensor(tensor &&rhs_tensor) = default;

        template <size_type... rhs_dims>
        tensor(tensor<data_type, rhs_dims...> &&rhs_tensor) : _dims{dims...}
        {
            this->_diff_type_move_initialize(std::move(rhs_tensor));
        }

        tensor &operator=(const tensor &rhs_tensor) = default;

        template <size_type... rhs_dims>
        tensor &operator=(const tensor<data_type, rhs_dims...> &rhs_tensor)
        {
            this->_diff_type_copy_assgin(rhs_tensor);
            return *this;
        }

        tensor &operator=(tensor &&rhs_tensor) = default;

        template <size_type... rhs_dims>
        tensor &operator=(tensor<data_type, rhs_dims...> &&rhs_tensor)
        {
            this->_diff_type_move_assgin(rhs_tensor);
            return *this;
        }

        ~tensor() = default;

        size_type order() const noexcept
        {
            return this->_dims.size();
        }

        size_type size() const noexcept
        {
            return this->_data.size();
        }

        std::vector<size_type> shape() const noexcept
        {
            return this->_dims;
        }

        size_type shape(size_type dim_index) const
        {
            return this->_dims[details::index_check(dim_index, this->_dims.size())];
        }

        auto begin() const noexcept
        {
            return this->_data.begin();
        }

        auto end() const noexcept
        {
            return this->_data.end();
        }

        template <typename... T>
        data_type &operator()(T... indices)
        {
            std::vector<size_type> index_list{indices...};
            size_type n_indices = index_list.size();

            if (n_indices != this->order())
                throw std::runtime_error("The number of indices must be equal to the tensor's order.");

            size_type index_1d = 0;
            size_type size_1d = this->size();
            size_type current_dim = 0;

            for (current_dim = 0; current_dim < n_indices; ++current_dim)
                index_1d += details::index_check(index_list[current_dim], _dims[current_dim]) * (size_1d /= _dims[current_dim]);

            return this->_data[index_1d];
        }

        friend std::ostream &operator<<(std::ostream &os, const tensor &rhs_tensor)
        {
            return os;
        }

    private:
        std::vector<data_type> _data;
        std::vector<size_type> _dims;

        void _initialize()
        {
            size_type size_1d = 1;
            std::for_each(this->_dims.begin(), this->_dims.end(), [&](int size)
                          { if (size <= 0)
                                    throw std::runtime_error("dim <= 0");
                                size_1d *= size; });          // exception: dim <= 0
            this->_data.resize(size_1d); // exception: bad_alloc
        }

        template <size_type... rhs_dims>
        void _diff_type_copy_initialize(const tensor<data_type, rhs_dims...> &rhs_tensor)
        {
            auto rhs_dim_vector = std::move(rhs_tensor.shape());
            if constexpr (is_dynamic_tensor::value == false)
                if (!std::equal(rhs_dim_vector.begin(), rhs_dim_vector.end(),
                                this->_dims.begin(), this->_dims.end()))
                    throw std::runtime_error("Dimensions mismatch.");

            this->_dims = std::move(rhs_dim_vector);
            this->_data.resize(rhs_tensor.size());
            std::copy(rhs_tensor.begin(), rhs_tensor.end(), this->_data.begin());
        }

        template <size_type... rhs_dims>
        void _diff_type_move_initialize(tensor<data_type, rhs_dims...> &&rhs_tensor)
        {
            auto rhs_dim_vector = std::move(rhs_tensor.shape());
            if constexpr (is_dynamic_tensor::value == false)
                if (!std::equal(rhs_dim_vector.begin(), rhs_dim_vector.end(),
                                this->_dims.begin(), this->_dims.end()))
                    throw std::runtime_error("Dimensions mismatch.");

            this->_dims = std::move(rhs_dim_vector);
            this->_data.resize(rhs_tensor.size());
            std::move(rhs_tensor.begin(), rhs_tensor.end(), this->_data.begin());
        }

        template <size_type... rhs_dims>
        void _diff_type_copy_assgin(const tensor<data_type, rhs_dims...> &rhs_tensor)
        {
            auto rhs_dim_vector = std::move(rhs_tensor.shape());
            if (!std::equal(rhs_dim_vector.begin(), rhs_dim_vector.end(),
                            this->_dims.begin(), this->_dims.end()))
            {
                if constexpr (is_dynamic_tensor::value == false)
                    throw std::runtime_error("Dimensions mismatch.");
                this->_data.resize(rhs_tensor.size());
            }

            this->_dims = std::move(rhs_dim_vector);
            std::copy(rhs_tensor.begin(), rhs_tensor.end(), this->_data.begin());
        }

        template <size_type... rhs_dims>
        void _diff_type_move_assign(tensor<data_type, rhs_dims...> &&rhs_tensor)
        {
            auto rhs_dim_vector = std::move(rhs_tensor.shape());
            if (!std::equal(rhs_dim_vector.begin(), rhs_dim_vector.end(),
                            this->_dims.begin(), this->_dims.end()))
            {
                if constexpr (is_dynamic_tensor::value == false)
                    throw std::runtime_error("Dimensions mismatch.");
                this->_data.resize(rhs_tensor.size());
            }

            this->_dims = std::move(rhs_dim_vector);
            std::move(rhs_tensor.begin(), rhs_tensor.end(), this->_data.begin());
        }
    };

} // namespace math::algebra::linear

#endif // MATH_ALGEBRA_LINEAR_TENSOR_HPP