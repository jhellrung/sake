/*******************************************************************************
 * sake/boost_ext/mpl/uint.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::mpl::uint<N>
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_MPL_UINT_HPP
#define SAKE_BOOST_EXT_MPL_UINT_HPP

#include <boost/mpl/integral_c.hpp>

namespace sake
{

namespace boost_ext
{

namespace mpl
{

template< unsigned int N >
struct uint
    : boost::mpl::integral_c< unsigned int, N >
{ };

} // namespace mpl

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_MPL_UINT_HPP
