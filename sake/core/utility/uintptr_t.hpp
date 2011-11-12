/*******************************************************************************
 * sake/core/utility/uintptr_t.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * typedef ... uintptr_t
 *
 * uintptr_t is a typedef for an unspecified unsigned integral type with the
 * same size as a void*.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_UINTPTR_T_HPP
#define SAKE_CORE_UTILITY_UINTPTR_T_HPP

#include <limits>

#include <boost/integer.hpp>
#include <boost/mpl/assert.hpp>

namespace sake
{

typedef boost::uint_t<
    sizeof( void* ) * std::numeric_limits< unsigned char >::digits
>::exact uintptr_t;

BOOST_STATIC_ASSERT((sizeof( void* ) == sizeof( uintptr_t )));

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_UINTPTR_T_HPP
