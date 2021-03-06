/*******************************************************************************
 * sake/core/range/algorithm/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ALGORITHM_FWD_HPP
#define SAKE_CORE_RANGE_ALGORITHM_FWD_HPP

#include <sake/core/functional/operators/fwd.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>

namespace sake
{

namespace range
{

namespace algorithm
{

namespace result_of
{

template< class R, class F >
class for_each;

template<
    class R,
    class T = typename sake::range_forward_traits<R>::value_type,
    class F = sake::operators::functional::plus
>
struct accumulate;

} // namespace result_of

namespace functional
{

struct accumulate;

struct for_each;

} // namespace functional

} // namespace algorithm

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ALGORITHM_FWD_HPP
