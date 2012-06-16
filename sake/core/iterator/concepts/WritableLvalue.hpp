/*******************************************************************************
 * sake/core/iterator/concepts/WritableLvalue.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct iterator::concepts::WritableLvalue<I>
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_LVALUE_HPP
#define SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_LVALUE_HPP

#include <sake/core/iterator/concepts/Lvalue.hpp>
#include <sake/core/iterator/concepts/Writable.hpp>

namespace sake
{

namespace iterator
{

namespace concepts
{

template< class I >
struct WritableLvalue
    : sake::iterator::concepts::Writable<I>,
      sake::iterator::concepts::Lvalue<I>
{ };

} // namespace concepts

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_CONCEPTS_WRITABLE_LVALUE_HPP
