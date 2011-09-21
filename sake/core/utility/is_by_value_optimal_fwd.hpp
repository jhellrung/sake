/*******************************************************************************
 * sake/core/utility/is_by_value_optimal_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_IS_BY_VALUE_OPTIMAL_FWD_HPP
#define SAKE_CORE_UTILITY_IS_BY_VALUE_OPTIMAL_FWD_HPP

namespace sake
{

template< class T >
struct is_by_value_optimal;

namespace ext
{

template< class T, class Enable >
struct is_by_value_optimal;

} // namespace ext

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_IS_BY_VALUE_OPTIMAL_FWD_HPP
