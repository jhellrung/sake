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

namespace sake
{

namespace boost_ext
{

namespace mpl
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class T, T... N >
struct vector_c
{ };

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_VECTOR_C_HPP
