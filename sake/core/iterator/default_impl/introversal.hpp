/*******************************************************************************
 * sake/core/iterator/default_impl/introversal.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_INTROVERSAL_HPP
#define SAKE_CORE_ITERATOR_DEFAULT_IMPL_INTROVERSAL_HPP

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/iterator/categories.hpp>

namespace sake
{

namespace iterator
{

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    has_type_iterator_introversal,
    iterator_introversal
)

namespace introversal_private
{

template< class I, bool = has_type_iterator_introversal<I>::value >
struct dispatch;

template< class I >
struct dispatch< I, false >
{ typedef sake::null_introversal_tag type; };

template< class I >
struct dispatch< I, true >
{ typedef typename I::iterator_introversal type; };

} // namespace introversal_private

template< class I >
struct introversal
    : introversal_private::dispatch<I>
{ };

} // namespace default_impl

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_DEFAULT_IMPL_INTROVERSAL_HPP
