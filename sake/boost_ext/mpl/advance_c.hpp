/*******************************************************************************
 * sake/boost_ext/mpl/advance_c.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::advance_c< Iterator, N >
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_ADVANCE_C_HPP
#define SAKE_BOOST_EXT_MPL_ADVANCE_C_HPP

#include <boost/mpl/advance.hpp>
#include <boost/type_traits/integral_constant.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< class Iterator, int N >
struct advance_c
    : boost::mpl::advance< Iterator, boost::integral_constant< int, N > >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_ADVANCE_C_HPP
