/*******************************************************************************
 * sake/core/iterator/private/at_static_assert_cond.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_ITERATOR_PRIVATE_AT_STATIC_ASSERT_COND_HPP
#define SAKE_CORE_ITERATOR_PRIVATE_AT_STATIC_ASSERT_COND_HPP

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/iterator/private/fwd.hpp>
#include <sake/core/iterator/private/is_convertible_relax.hpp>
#include <sake/core/iterator/traits.hpp>
#include <sake/core/iterator/traits_fwd.hpp>

namespace sake
{

namespace iterator
{

namespace private_
{

template< class I, class T >
struct at_static_assert_cond_dispatch
    : sake::iterator::private_::is_convertible_relax<T,I>
{ };

template< class I >
struct at_static_assert_cond_dispatch< I, sake::begin_tag >
    : boost_ext::is_convertible<
          typename sake::iterator_introterminal<I>::type,
          sake::begin_access_introterminal_tag
      >
{ };

template< class I >
struct at_static_assert_cond_dispatch< I, sake::end_tag >
    : boost_ext::is_convertible<
          typename sake::iterator_introterminal<I>::type,
          sake::end_access_introterminal_tag
      >
{ };

template< class I, class T >
struct at_static_assert_cond
    : private_::at_static_assert_cond_dispatch<
          typename boost_ext::remove_qualifiers<I>::type,
          typename boost_ext::remove_qualifiers<T>::type
      >
{ };

} // namespace private_

} // namespace iterator

} // namespace sake

#endif // #ifndef SAKE_CORE_ITERATOR_PRIVATE_AT_STATIC_ASSERT_COND_HPP
