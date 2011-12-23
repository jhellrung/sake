/*******************************************************************************
 * sake/core/memory/alloc/rebind_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MEMORY_ALLOC_REBIND_FWD_HPP
#define SAKE_CORE_MEMORY_ALLOC_REBIND_FWD_HPP

namespace sake
{

namespace alloc
{

template< class A, class T >
struct rebind;

namespace extension
{
template< class A, class T, class Enable = void >
struct rebind;
} // namespace extension

namespace default_impl
{
template< class A, class T >
struct rebind;
} // namespace default_impl

} // namespace alloc

} // namespace sake

#endif // #ifndef SAKE_CORE_MEMORY_ALLOC_REBIND_FWD_HPP
