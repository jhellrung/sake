/*******************************************************************************
 * sake/core/utility/pointee_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_POINTEE_FWD_HPP
#define SAKE_CORE_UTILITY_POINTEE_FWD_HPP

namespace sake
{

template< class P >
struct pointee;

namespace extension
{
template< class P, class Enable = void >
struct pointee;
} // namespace extension

namespace default_impl
{
template< class P >
struct pointee;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_POINTEE_FWD_HPP
