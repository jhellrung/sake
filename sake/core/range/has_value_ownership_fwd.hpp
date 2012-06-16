/*******************************************************************************
 * sake/core/range/has_value_ownership_fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_HAS_VALUE_OWNERSHIP_FWD_HPP
#define SAKE_CORE_RANGE_HAS_VALUE_OWNERSHIP_FWD_HPP

namespace sake
{

namespace range
{

template< class R >
struct has_value_ownership;

namespace extension
{
template< class R, class Enable = void >
struct has_value_ownership;
} // namespace extension

namespace default_impl
{
template< class R >
struct has_value_ownership;
} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_HAS_VALUE_OWNERSHIP_FWD_HPP
