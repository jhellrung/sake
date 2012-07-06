/*******************************************************************************
 * sake/core/range/concepts/fwd.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_FWD_HPP
#define SAKE_CORE_RANGE_CONCEPTS_FWD_HPP

#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

namespace concepts
{

template< class R >
struct Range;

} // namespace concepts

namespace range
{

namespace concepts
{

namespace private_
{

template< class R >
struct Base;

template< class Traversal >
struct TraversalBase_index;

template<
    class R,
    class Traversal = typename sake::range_traversal<R>::type,
    int = TraversalBase_index< Traversal >::value
>
struct TraversalBase;

} // namespace private_

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_FWD_HPP
