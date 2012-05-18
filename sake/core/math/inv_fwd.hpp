/*******************************************************************************
 * sake/core/math/inv_fwd.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_MATH_INV_FWD_HPP
#define SAKE_CORE_MATH_INV_FWD_HPP

namespace sake
{

namespace result_of
{

template< class T >
struct inv;

namespace extension
{
template< class T, class Enable = void >
struct inv;
} // namespace extension

namespace default_impl
{
template< class T >
struct inv;
} // namespace default_impl

} // namespace result_of

namespace functional
{

struct inv;

} // namespace functional

} // namespace sake

#endif // #ifndef SAKE_CORE_MATH_INV_FWD_HPP
