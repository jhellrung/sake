/*******************************************************************************
 * sake/core/range/apply/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_APPLY_FWD_HPP
#define SAKE_CORE_RANGE_APPLY_FWD_HPP

#include <sake/core/functional/operators/fwd.hpp>

namespace sake
{

namespace range
{

namespace apply
{

namespace functional
{

template< class T = void, class F = sake::operators::functional::plus >
struct accumulate;

template< class F = void >
struct for_each;

} // namespace functional

} // namespace apply

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_APPLY_FWD_HPP
