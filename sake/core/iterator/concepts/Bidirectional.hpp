/*******************************************************************************
 * sake/core/iterator/concepts/Bidirectional.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::concepts::Bidirectional<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_BIDIRECTIONAL_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_BIDIRECTIONAL_HPP

#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/concepts/private/TraversalBase.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

template< class I >
struct Bidirectional
    : sake::iterator::concepts::private_::TraversalBase<
          I, boost::bidirectional_traversal_tag >
{ };

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_BIDIRECTIONAL_HPP
