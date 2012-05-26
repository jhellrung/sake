/*******************************************************************************
 * sake/core/iterator/concepts/ReadableLvalue.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator_concepts::ReadableLvalue<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_READABLE_LVALUE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_READABLE_LVALUE_HPP

#include <sake/core/iterator/concepts/Lvalue.hpp>

namespace sake
{

namespace iterator_concepts
{

template< class I >
struct ReadableLvalue
    : sake::iterator_concepts::Lvalue<I>
{ };

} // namespace iterator_concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_READABLE_LVALUE_HPP
