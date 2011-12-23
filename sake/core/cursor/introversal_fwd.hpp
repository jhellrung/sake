/*******************************************************************************
 * sake/core/cursor/introversal_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_CURSOR_INTROVERSAL_FWD_HPP
#define SAKE_CORE_CURSOR_INTROVERSAL_FWD_HPP

namespace sake
{

template< class C >
struct cursor_introversal;

namespace extension
{
template< class C, class Enable = void >
struct cursor_introversal;
} // namespace extension

namespace default_impl
{
template< class C >
struct cursor_introversal;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_CURSOR_INTROVERSAL_FWD_HPP
