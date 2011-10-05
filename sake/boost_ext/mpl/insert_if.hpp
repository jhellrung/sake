/*******************************************************************************
 * sake/boost_ext/mpl/insert_if.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::insert_if< C, Sequence, T >
 * struct boost_ext::mpl::insert_if_c< C, Sequence, T >
 * struct boost_ext::mpl::lazy_insert_if< C, Sequence, T >
 * struct boost_ext::mpl::lazy_insert_if_c< C, Sequence, T >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_INSERT_IF_HPP
#define SAKE_BOOST_EXT_MPL_INSERT_IF_HPP

#include <boost/mpl/insert.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< bool C, class Sequence, class T >
struct insert_if_c;

template< class Sequence, class T >
struct insert_if_ic< false, Sequence, T >
{ typedef Sequence type; };

template< class Sequence, class T >
struct insert_if_c< true, Sequence, T >
    : boost::mpl::insert< Sequence, T >
{ };

template< class C, class Sequence, class T >
struct insert_if
    : insert_if_c< C::value, Sequence, T >
{ };

template< bool C, class Sequence, class T >
struct lazy_insert_if_c;

template< class Sequence, class T >
struct lazy_insert_if_c< false, Sequence, T >
{ typedef Sequence type; };

template< class Sequence, class T >
struct lazy_insert_if_c< true, Sequence, T >
    : boost::mpl::insert< Sequence, typename T::type >
{ };

template< class C, class Sequence, class T >
struct lazy_insert_if
    : lazy_insert_if_c< C::value, Sequence, T >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_INSERT_IF_HPP
