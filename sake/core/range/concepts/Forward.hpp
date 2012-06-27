/*******************************************************************************
 * sake/core/range/concepts/Forward.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::concepts::Forward<R>
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_CONCEPTS_FORWARD_HPP
#define SAKE_CORE_RANGE_CONCEPTS_FORWARD_HPP

#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/private/TraversalBase.hpp>

namespace sake
{

namespace range
{

namespace concepts
{

template< class R >
struct Forward
    : sake::range::concepts::private_::TraversalBase<
          R, boost::forward_traversal_tag >
{ };

} // namespace concepts

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_CONCEPTS_FORWARD_HPP
