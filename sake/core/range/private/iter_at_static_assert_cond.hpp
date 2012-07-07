/*******************************************************************************
 * sake/core/range/private/iter_at_static_assert_cond.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_PRIVATE_ITER_AT_STATIC_ASSERT_COND_HPP
#define SAKE_CORE_RANGE_PRIVATE_ITER_AT_STATIC_ASSERT_COND_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/iterator/begin_end_tag.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>

namespace sake
{

namespace range
{

namespace private_
{

template< class R, class T >
struct iter_at_static_assert_cond_dispatch
    : boost_ext::is_convertible<
          T, typename sake::range_forward_iterator<R>::type >
{ };

template< class R >
struct iter_at_static_assert_cond_dispatch< R, sake::begin_tag >
    : boost::true_type
{ };

template< class R >
struct iter_at_static_assert_cond_dispatch< R, sake::end_tag >
    : boost::true_type
{ };

template< class R, class T >
struct iter_at_static_assert_cond
    : private_::iter_at_static_assert_cond_dispatch<
          R, typename boost_ext::remove_qualifiers<T>::type >
{ };

} // namespace private_

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_PRIVATE_ITER_AT_STATIC_ASSERT_COND_HPP
