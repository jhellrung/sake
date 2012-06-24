/*******************************************************************************
 * sake/core/range/concepts/Range.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_RANGE_HPP
#define SAKE_CORE_RANGE_CONCEPTS_RANGE_HPP

//#include <sake/core/range/concepts/private/TraversalBase.hpp>

namespace sake
{

namespace concepts
{

template< class R >
struct Range
//    : sake::range::concepts::private_::TraversalBase<R>
{ };

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_RANGE_HPP
