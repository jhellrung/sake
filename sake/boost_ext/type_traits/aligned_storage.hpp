/*******************************************************************************
 * sake/boost_ext/type_traits/aligned_storage.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::aligned_storage< std::size_t Size, std::size_t Align >
 *
 * boost_ext::aligned_storage is similar to boost::aligned_storage.
 * aligned_storage< size, alignment >::type is a union with a character array
 * member _, which may be used to access the underlying bytes.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_STORAGE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_STORAGE_HPP

#include <cstddef>

#include <sake/boost_ext/type_traits/aligned_type.hpp>

namespace sake
{

template< std::size_t Size, std::size_t Align >
union aligned_storage
{
    typedef aligned_storage type;
    unsigned char _[Size];
    typename boost_ext::aligned_type< Align >::type _aligned;
};

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_STORAGE_HPP
