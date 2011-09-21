/*******************************************************************************
 * sake/core/utility/aligned_storage.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct aligned_storage< std::size_t size, std::size_t alignment >
 *
 * aligned_storage is similar to boost::aligned_storage.
 * aligned_storage< size, alignment >::type is a union with a character array
 * member _, which may be used to access the underlying bytes.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_ALIGNED_STORAGE_HPP
#define SAKE_CORE_UTILITY_ALIGNED_STORAGE_HPP

#include <cstddef>

#include <boost/type_traits/type_with_alignment.hpp>

namespace sake
{

template< std::size_t size, std::size_t alignment >
struct aligned_storage
{
    union type
    {
        unsigned char _[size];
        typename boost::type_with_alignment< alignment >::type _aligned;
    };
};

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_ALIGNED_STORAGE_HPP
