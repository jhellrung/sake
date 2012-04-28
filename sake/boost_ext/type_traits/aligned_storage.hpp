/*******************************************************************************
 * sake/boost_ext/type_traits/aligned_storage.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::aligned_storage_c< std::size_t Size, std::size_t Align >
 * struct boost_ext::aligned_storage<T>
 *
 * boost_ext::aligned_storage_c is similar to boost::aligned_storage.
 * aligned_storage_c< size, alignment >::type is a union with a character array
 * member _, which may be used to access the underlying bytes.
 *
 * boost_ext::aligned_storage<T>::type is equivalent to
 *     boost_ext::aligned_storage_c<
 *         sizeof( T ),
 *         boost::alignment_of<T>::value
 *     >::type
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_STORAGE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_STORAGE_HPP

#include <cstddef>

#include <boost/type_traits/alignment_of.hpp>

#include <sake/boost_ext/type_traits/aligned_type.hpp>

namespace sake
{

namespace boost_ext
{

template< std::size_t Size, std::size_t Align >
union aligned_storage_c
{
    typedef aligned_storage_c type;
    unsigned char _[Size];
    typename boost_ext::aligned_type< Align >::type _aligned;
}
#ifdef __GNUC__
__attribute__ (( __may_alias__ ))
#endif // #ifdef __GNUC__
;

template< class T >
struct aligned_storage
{ typedef aligned_storage_c< sizeof( T ), boost::alignment_of<T>::value > type; };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ALIGNED_STORAGE_HPP
