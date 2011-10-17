/*******************************************************************************
 * sake/core/math/constants_traits_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_CONSTANTS_TRAITS_FWD_HPP
#define SAKE_CORE_MATH_CONSTANTS_TRAITS_FWD_HPP

namespace sake
{

template< class T >
struct constants_traits;

namespace extension
{
template< class T, class Enable = void >
struct constants_traits;
} // namespace extension

namespace default_impl
{
template< class T >
struct constants_traits;
} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_NUMERIC_TRAITS_FWD_HPP
