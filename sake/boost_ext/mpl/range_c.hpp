/*******************************************************************************
 * sake/boost_ext/mpl/range_c.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::range_c< T, Lo, Hi >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_RANGE_C_HPP
#define SAKE_BOOST_EXT_MPL_RANGE_C_HPP

#include <boost/config.hpp>

#include <sake/boost_ext/mpl/vector_c.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class T, T Lo, T Hi >
struct range_c;

namespace range_c_private
{

template< class T, class VectorC, T M >
struct push_front;

template< class T, T... N, T M >
struct push_front< boost_ext::mpl::vector_c< T, N... >, M >
{ typedef boost_ext::mpl::vector_c< T, M, N... > type; };

} // namespace range_c_private

template< class T, T Lo, T Hi >
struct range_c
    : range_c_private::push_front<
          typename range_c< T, Lo + 1, Hi >::type,
          Lo
      >
{ };

template< class T, T N >
struct range_c< T, N, N >
{ typedef boost_ext::mpl::vector_c<T> type; };

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_RANGE_C_HPP
