/*******************************************************************************
 * sake/boost_ext/mpl/vector_c.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::vector_c< T, N... >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_VECTOR_C_HPP
#define SAKE_BOOST_EXT_MPL_VECTOR_C_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_VARIADIC_TEMPLATES

#include <boost/mpl/integral_c.hpp>

#include <sake/boost_ext/mpl/vector.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class T, T... N >
struct vector_c
    : boost_ext::mpl::vector< boost::mpl::integral_c<T,N>... >
{ typedef T value_type; };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#endif // #ifndef SAKE_BOOST_EXT_MPL_VECTOR_C_HPP
