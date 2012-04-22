/*******************************************************************************
 * sake/boost_ext/type_traits/add_const_if.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_const_if_c< Cond, T >
 * struct boost_ext::add_const_if< Cond, T >
 * struct boost_ext::add_const_if_not< Cond, T >
 *
 * This conditionally adds a const qualifier to T.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_IF_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_IF_HPP

#include <boost/type_traits/add_const.hpp>

namespace sake
{

namespace boost_ext
{

template< bool Cond, class T >
struct add_const_if_c;

template< class T >
struct add_const_if_c< false, T >
{ typedef T type; };

template< class T >
struct add_const_if_c< true, T >
    : boost::add_const<T>
{ };

template< class Cond, class T >
struct add_const_if
    : add_const_if_c< Cond::value, T >
{ };

template< class Cond, class T >
struct add_const_if_not
    : add_const_if_c< !Cond::value, T >
{ };

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_IF_HPP
