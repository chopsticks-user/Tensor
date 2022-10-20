#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <string>

#define ALLOW_NEGATIVE_INDEX 1

namespace exception
{
    std::string exception_message(std::string &&error_message)
    {
        std::string full_error_message{__FILE__};
        full_error_message += ':';
        full_error_message += std::to_string(__LINE__ + 4);
        full_error_message += ": ";
        full_error_message += error_message;
        return full_error_message;
    }

}

namespace container
{
    namespace details
    {
#if ALLOW_NEGATIVE_INDEX == 0
        typedef size_t container_size_type;
#else
        typedef ssize_t container_size_type;
#endif
#if ALLOW_NEGATIVE_INDEX == 0
        container_size_type index_check(container_size_type index,
                                        container_size_type oor_index)
        {
            if (index >= oor_index || index < 0)
                throw std::runtime_error("Index out of range.");
            return index;
        }
#else
        container_size_type index_check(container_size_type index,
                                        container_size_type oor_index)
        {
            if (index >= oor_index || index < -oor_index)
                throw std::runtime_error("Index out of range.");
            return index >= 0 ? index : oor_index + index;
        }
#endif

        template <typename data_type, container_size_type... dims>
        class tensor_base_static
        {
        public:
            tensor_base_static()
            {
                container_size_type size_1d = 1;
                std::for_each(_dims.begin(), _dims.end(), [&](int size)
                              { size_1d *= size; }); // exception: dim <= 0
                _data.resize(size_1d);               // exception: bad_alloc
            }

            container_size_type order() const noexcept
            {
                return _dims.size();
            }

            container_size_type size() const noexcept
            {
                return _data.size();
            }

            std::vector<container_size_type> shape() const
            {
                return this->_dims;
            }

            container_size_type shape(container_size_type dim_index) const
            {
                return _dims[index_check(dim_index, _dims.size())];
            }

            template <typename... T>
            data_type &operator()(T... indices)
            {
                // auto index_list = std::make_tuple(indices...);
                std::vector<container_size_type> index_list{indices...};
                container_size_type n_indices = index_list.size();

                if (n_indices != this->order())
                    throw std::runtime_error(exception::exception_message("The number of indices must be equal to the tensor's order."));

                container_size_type index_1d = 0;
                container_size_type size_1d = this->size();
                container_size_type current_dim = 0;

                for (current_dim = 0; current_dim < n_indices; ++current_dim)
                    index_1d += index_check(index_list[current_dim], _dims[current_dim]) * (size_1d /= _dims[current_dim]);

                return _data[index_1d];
            }

            friend std::ostream &operator<<(std::ostream &os, const tensor_base_static &rhs_static_tensor)
            {
                return os;
            }

            std::vector<data_type> _data;

        protected:
            std::vector<container_size_type> _dims{dims...};
        };

    } // namespace details

} // namespace container

#endif // TENSOR_HPP