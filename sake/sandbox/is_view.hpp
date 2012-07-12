/*******************************************************************************
 * sake/core/range/is_view.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_is_view<R>
 * struct extension::range_is_view< R, Enable = void >
 * struct default_impl::range_is_view<R>
 *
 * range_is_view is a metafunction indicating if a range is a view range.  A
 * view range does not own its elements (i.e., it has reference semantics)
 * and should be cheap to copy (hence, e.g., it may be efficiently stored by
 * value).
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_IS_VIEW_HPP
#define SAKE_CORE_RANGE_IS_VIEW_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/range/is_view_fwd.hpp>

namespace sake
{

template< class R >
struct range_is_view
    : sake::extension::range_is_view<
          typename boost::remove_const<R>::type >
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct range_is_view
    : sake::default_impl::range_is_view<R>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    range_has_type_is_view_tag,
    is_view_tag
)

template< class R >
struct range_is_view
    : sake::default_impl::range_has_type_is_view_tag<
          R, boost::mpl::quote1< boost::mpl::identity > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_IS_VIEW_HPP
