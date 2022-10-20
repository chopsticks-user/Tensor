#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

#define ALLOW_NEGATIVE_INDEX 1

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

                if (n_indices > this->order())
                    throw std::runtime_error("Too many arguments.");

                container_size_type index_1d = 0;
                container_size_type size_1d = this->size();

                if (n_indices == this->order())
                    for (container_size_type i = 0; i < n_indices; ++i)
                        index_1d += index_check(index_list[i], _dims[i]) * (size_1d /= _dims[i]);

                
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