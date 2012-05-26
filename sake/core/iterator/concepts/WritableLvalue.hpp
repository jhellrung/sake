/*******************************************************************************
 * sake/core/iterator/concepts/WritableLvalue.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::WritableLvalue<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_LVALUE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_LVALUE_HPP

#include <sake/core/iterator/concepts/Lvalue.hpp>
#include <sake/core/iterator/concepts/Writable.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct WritableLvalue
    : sake::iterator_concepts::Writable<I>,
      sake::iterator_concepts::Lvalue<I>
{ };

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_LVALUE_HPP
