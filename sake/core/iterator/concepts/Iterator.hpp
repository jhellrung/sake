/*******************************************************************************
 * sake/core/iterator/concepts/Iterator.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_ITERATOR_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_ITERATOR_HPP

#include <sake/core/iterator/concepts/private/IntroversalBase.hpp>
#include <sake/core/iterator/concepts/private/TraversalBase.hpp>

namespace sake
{

namespace concepts
{

template< class I >
struct Iterator
    : sake::iterator::concepts::private_::TraversalBase<I>,
      sake::iterator::concepts::private_::IntroversalBase<I>
{ };

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_ITERATOR_HPP
