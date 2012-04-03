/*******************************************************************************
 * sake/core/utility/is_convertible_wnrbt_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IS_CONVERTIBLE_WNRBT_FWD_HPP
#define SAKE_CORE_UTILITY_IS_CONVERTIBLE_WNRBT_FWD_HPP

namespace sake
{

template< class From, class To >
struct is_convertible_wnrbt;

namespace extension
{
template< class From, class To, class Enable = void >
struct is_convertible_wnrbt;
} // namespace extension

namespace default_impl
{
template< class From, class To >
struct is_convertible_wnrbt;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IS_CONVERTIBLE_WNRBT_FWD_HPP
