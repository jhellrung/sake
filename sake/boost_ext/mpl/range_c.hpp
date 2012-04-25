/*******************************************************************************
 * sake/boost_ext/mpl/range_c.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::range_c<T,A,B>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_RANGE_C_HPP
#define SAKE_BOOST_EXT_MPL_RANGE_C_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_VARIADIC_TEMPLATES

#include <boost/mpl/integral_c.hpp>

#include <sake/boost_ext/mpl/vector.hpp>
#include <sake/boost_ext/mpl/vector_c.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class T, T A, T B >
struct range_c;

namespace range_c_private
{

template< class T, class VectorC, T M >
struct push_front;

template< class T, T... N, T M >
struct push_front<
    boost_ext::mpl::vector< boost::mpl::integral_c<T,N>... >, M >
{ typedef boost_ext::mpl::vector_c< T, M, N... > type; };

} // namespace range_c_private

template< class T, T A, T B >
struct range_c
    : range_c_private::push_front<
          typename boost_ext::mpl::range_c< T, A + 1, B >::type,
          A
      >::type
{ };

template< class T, T N >
struct range_c< T, N, N >
    : boost_ext::mpl::vector_c<T>
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#endif // #ifndef SAKE_BOOST_EXT_MPL_RANGE_C_HPP
