/*******************************************************************************
 * sake/boost_ext/mpl/insert_if.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::insert_if_c< Cond, Sequence, T >
 * struct boost_ext::mpl::insert_if< Cond, Sequence, T >
 * struct boost_ext::mpl::insert_if_not_c< Cond, Sequence, T >
 * struct boost_ext::mpl::insert_if_not< Cond, Sequence, T >
 * struct boost_ext::mpl::lazy_insert_if_c< Cond, Sequence, T >
 * struct boost_ext::mpl::lazy_insert_if< Cond, Sequence, T >
 * struct boost_ext::mpl::lazy_insert_if_not_c< Cond, Sequence, T >
 * struct boost_ext::mpl::lazy_insert_if_not< Cond, Sequence, T >
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

template< bool Cond, class Sequence, class T >
struct insert_if_c;

template< class Sequence, class T >
struct insert_if_c< false, Sequence, T >
{ typedef Sequence type; };

template< class Sequence, class T >
struct insert_if_c< true, Sequence, T >
    : boost::mpl::insert< Sequence, T >
{ };

template< class Cond, class Sequence, class T >
struct insert_if
    : insert_if_c< Cond::value, Sequence, T >
{ };

template< bool Cond, class Sequence, class T >
struct insert_if_not_c
    : insert_if_c< !Cond, Sequence, T >
{ };

template< class Cond, class Sequence, class T >
struct insert_if_not
    : insert_if_c< !Cond::value, Sequence, T >
{ };

template< bool Cond, class Sequence, class T >
struct lazy_insert_if_c;

template< class Sequence, class T >
struct lazy_insert_if_c< false, Sequence, T >
{ typedef Sequence type; };

template< class Sequence, class T >
struct lazy_insert_if_c< true, Sequence, T >
    : boost::mpl::insert< Sequence, typename T::type >
{ };

template< class Cond, class Sequence, class T >
struct lazy_insert_if
    : lazy_insert_if_c< Cond::value, Sequence, T >
{ };

template< bool Cond, class Sequence, class T >
struct lazy_insert_if_not_c
    : lazy_insert_if_c< !Cond, Sequence, T >
{ };

template< class Cond, class Sequence, class T >
struct lazy_insert_if_not
    : lazy_insert_if_c< !Cond::value, Sequence, T >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_INSERT_IF_HPP
