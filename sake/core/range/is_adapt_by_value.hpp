/*******************************************************************************
 * sake/core/range/is_adapt_by_value.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_is_adapt_by_value<R>
 * struct extension::range_is_adapt_by_value< R, Enable = void >
 * struct default_impl::range_is_adapt_by_value<R>
 *
 * range_is_adapt_by_value is a metafunction indicating if a range should be
 * held by value in range adaptors.
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_IS_ADAPT_BY_VALUE_HPP
#define SAKE_CORE_RANGE_IS_ADAPT_BY_VALUE_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/range/is_adapt_by_value_fwd.hpp>

namespace sake
{

template< class R >
struct range_is_adapt_by_value< R& >
    : boost::true_type
{ };

template< class R >
struct range_is_adapt_by_value
    : sake::extension::range_is_adapt_by_value<
          typename boost::remove_const<R>::type >
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct range_is_adapt_by_value
    : sake::default_impl::range_is_adapt_by_value<R>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   range_has_type_is_adapt_by_value_tag,
   is_adapt_by_value_tag
)

template< class R >
struct range_is_adapt_by_value
    : sake::default_impl::range_has_type_is_adapt_by_value_tag<
          R, boost::mpl::quote1< boost::mpl::identity > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_IS_ADAPT_BY_VALUE_HPP
