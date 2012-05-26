/*******************************************************************************
 * sake/core/iterator/concepts/ReadableWritable.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::ReadableWritable<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_READABLE_WRITABLE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_READABLE_WRITABLE_HPP

#include <sake/core/iterator/concepts/Readable.hpp>
#include <sake/core/iterator/concepts/Swappable.hpp>
#include <sake/core/iterator/concepts/Writable.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct ReadableWritable
    : sake::iterator_concepts::Readable<I>,
      sake::iterator_concepts::Writable<I>,
      sake::iterator_concepts::Swappable<I>
{ };

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_READABLE_WRITABLE_HPP
