/*******************************************************************************
 * sake/core/range/multidim_traits_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_MULTIDIM_TRAITS_FWD_HPP
#define SAKE_CORE_RANGE_MULTIDIM_TRAITS_FWD_HPP

namespace sake
{

template< class R >
struct range_multidim_traits;

namespace extension
{
template< class R, class Enable = void >
struct range_multidim_traits;
} // namespace extension

namespace default_impl
{
template< class R >
struct range_multidim_traits;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_MULTIDIM_TRAITS_FWD_HPP
