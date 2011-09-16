/*******************************************************************************
 * boost_ext/type_traits/has_small_size.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::has_small_size<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_HAS_SMALL_SIZE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_HAS_SMALL_SIZE_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>

namespace sake
{

namespace boost_ext
{

namespace has_small_size_private
{

template< class T, bool = boost::is_object<T>::value >
struct dispatch
    : boost::false_type
{ };

template< class T >
struct dispatch< T, true >
{
    static const bool value = sizeof( T ) <= sizeof( void* );
    typedef dispatch type;
};

} // namespace has_small_size_private

template< class T >
struct has_small_size
    : has_small_size_private::dispatch<T>
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_HAS_SMALL_SIZE_HPP
