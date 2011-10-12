/*******************************************************************************
 * sake/core/utility/rebind_pointer_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_REBIND_POINTER_FWD_HPP
#define SAKE_CORE_UTILITY_REBIND_POINTER_FWD_HPP

namespace sake
{

template< class P, class T >
struct rebind_pointer;

namespace extension
{
template< class P, class T, class Enable = void >
struct rebind_pointer;
} // namespace extension

namespace default_impl
{
template< class P >
struct rebind_pointer;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_REBIND_POINTER_FWD_HPP
