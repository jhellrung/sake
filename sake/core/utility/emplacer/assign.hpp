/*******************************************************************************
 * sake/core/utility/emplacer/assign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * emplacer_assign(T& x, U&& y) -> T&
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_EMPLACER_ASSIGN_HPP
#define SAKE_CORE_UTILITY_EMPLACER_ASSIGN_HPP

#include <sake/core/move/forward.hpp>

namespace sake
{

template< class T, class U >
inline T&
emplacer_assign(T& x, SAKE_FWD2_REF( U ) y)
{ return x = sake::forward<U>(y); }

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_EMPLACER_ASSIGN_HPP
