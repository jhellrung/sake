/*******************************************************************************
 * sake/core/range/has_value_ownership.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_has_value_ownership<R>
 * struct extension::range_has_value_ownership< R, Enable = void >
 * struct default_impl::range_has_value_ownership<R>
 *
 * range_has_value_ownership is a metafunction indicating if a range has
 * ownership of its values (i.e., has value semantics).  Such ranges should be
 * stored by reference and rvalue-ness may be propagated from the range to its
 * values.
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_HAS_VALUE_OWNERSHIP_HPP
#define SAKE_CORE_RANGE_HAS_VALUE_OWNERSHIP_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/range/has_value_ownership_fwd.hpp>

namespace sake
{

template< class R >
struct range_has_value_ownership< R& >
    : boost::false_type
{ };

template< class R >
struct range_has_value_ownership
    : sake::extension::range_has_value_ownership<
          typename boost::remove_const<R>::type >
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct range_has_value_ownership
    : sake::default_impl::range_has_value_ownership<R>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    range_has_type_has_value_ownership_tag,
    has_value_ownership_tag
)

template< class R >
struct range_has_value_ownership
    : sake::default_impl::range_has_type_has_value_ownership_tag<
          R, boost::mpl::quote1< boost::mpl::identity > >
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_HAS_VALUE_OWNERSHIP_FWD_HPP
