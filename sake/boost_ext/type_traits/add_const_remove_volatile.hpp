/*******************************************************************************
 * boost_ext/type_traits/add_const_remove_volatile.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_const_remove_volatile<T>
 *
 * add_const_remove_volatile<T> -> add_const<T>
 * add_const_remove_volatile< T volatile > -> add_const<T>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_REMOVE_VOLATILE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_REMOVE_VOLATILE_HPP

#include <boost/type_traits/add_const.hpp>

namespace sake
{

namespace boost_ext
{

template< class T >
struct add_const_remove_volatile
    : boost::add_const<T>
{ };

template< class T >
struct add_const_remove_volatile< T volatile >
    : boost::add_const<T>
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_REMOVE_VOLATILE_HPP
