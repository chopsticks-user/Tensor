#ifndef TENSOR_HPP
#define TENSOR_HPP

#include "../Config.hpp"
#include "../Utils/Utils.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>

namespace container
{
    // template <typename data_type, size_type... dims, allocator>
    template <typename data_type, size_type... dims>
    class tensor
    {
    public:
        tensor()
        {
            size_type size_1d = 1;
            std::for_each(_dims.begin(), _dims.end(), [&](int size)
                          { if (size <= 0)
                                    throw std::runtime_error("dim <= 0");
                                size_1d *= size; });    // exception: dim <= 0
            _data.resize(size_1d); // exception: bad_alloc
        }

        size_type order() const noexcept
        {
            return _dims.size();
        }

        size_type size() const noexcept
        {
            return _data.size();
        }

        std::vector<size_type> shape() const noexcept
        {
            return this->_dims;
        }

        size_type shape(size_type dim_index) const
        {
            return _dims[details::index_check(dim_index, _dims.size())];
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

            return _data[index_1d];
        }

        friend std::ostream &operator<<(std::ostream &os, const tensor &rhs_tensor)
        {
            return os;
        }

    private:
        std::vector<data_type> _data;
        std::vector<size_type> _dims{dims...};
    };

    template <typename data_type>
    class tensor<data_type>
    {
    public:
        tensor() = default;

        template <typename... T>
        tensor(T... dims) : _dims{dims...}
        {
            size_type size_1d = 1;
            std::for_each(_dims.begin(), _dims.end(), [&](int size)
                          { if (size <= 0)
                                    throw std::runtime_error("dim <= 0");
                                size_1d *= size; });    // exception: dim <= 0
            _data.resize(size_1d); // exception: bad_alloc
        }

        size_type order() const noexcept
        {
            return _dims.size();
        }

        size_type size() const noexcept
        {
            return _data.size();
        }

        std::vector<size_type> shape() const noexcept
        {
            return this->_dims;
        }

        size_type shape(size_type dim_index) const
        {
            return _dims[details::index_check(dim_index, _dims.size())];
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

            return _data[index_1d];
        }

        friend std::ostream &operator<<(std::ostream &os, const tensor &rhs_tensor)
        {
            return os;
        }

    private:
        std::vector<data_type> _data;
        std::vector<size_type> _dims;
    };

} // namespace container

#endif // TENSOR_HPP