/*******************************************************************************
 * sake/core/utility/sizeof_t.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct sizeof_t<N>
 *
 * sizeof( sizeof_t<N> ) == N.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_SIZEOF_T_HPP
#define SAKE_CORE_UTILITY_SIZEOF_T_HPP

#include <cstddef>

#include <boost/static_assert.hpp>

namespace sake
{

template< std::size_t N >
struct sizeof_t
{ char _[N]; };

BOOST_STATIC_ASSERT((sizeof( sizeof_t<1> ) == 1));
BOOST_STATIC_ASSERT((sizeof( sizeof_t<2> ) == 2));

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_SIZEOF_T_HPP
