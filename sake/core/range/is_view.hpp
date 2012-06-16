/*******************************************************************************
 * sake/core/range/is_view.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::is_view<R>
 * struct range::extension::is_view< R, Enable = void >
 * struct range::default_impl::is_view<R>
 *
 * is_view is a metafunction indicating if a range is a view range.  A view
 * range does not own its elements (i.e., it has reference semantics), should be
 * stored by value, and should be cheap to copy.
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_IS_VIEW_HPP
#define SAKE_CORE_RANGE_IS_VIEW_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/range/has_value_ownership.hpp>
#include <sake/core/range/is_view_fwd.hpp>
#include <sake/core/range/traits.hpp>

namespace sake
{

namespace range
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
   has_type_is_view_tag,
   is_view_tag
)

template< class R >
struct is_view
    : sake::range::extension::is_view<
          typename boost::remove_const<R>::type >
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct is_view
    : sake::range::default_impl::is_view<R>
{ };

} // namespace extension

namespace default_impl
{

template< class R >
struct is_view
    : boost::mpl::if_<
          sake::has_type_is_view_tag<R>,
          sake::has_type_is_view_tag<R,
              boost::mpl::quote1< boost::mpl::identity > >,
          boost_ext::mpl::and2<
              boost::mpl::not_< sake::range::has_value_ownership<R> >,
              boost::is_same<
                  typename sake::range_iterator<R>::type,
                  typename sake::range_iterator< R const >::type
              >
          >
      >::type
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_IS_VIEW_HPP
