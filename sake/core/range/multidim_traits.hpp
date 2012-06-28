/*******************************************************************************
 * sake/core/range/multidim_traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_multidim_traits<R>
 * struct extension::range_multidim_traits< R, Enable = void >
 * struct default_impl::range_multidim_traits<R>
 *
 * struct range_multidim_enable<R>
 * struct range_multidim_outer<R>
 *
 * range_multidim_traits has the following interface.
 *
 * template< class R >
 * struct range_multidim_traits
 * {
 *     typedef ... enable_tag;
 *
 *     // Only if enable_tag::value == true:
 *
 *     typedef ... outer_range;
 *
 *     static outer_range
 *     outer(R& r);
 *
 *     template< class Outer, class Inner, class Introversal >
 *     static typename range_iterator< R, Introversal >::type
 *     iter_at(R& r, Outer j, Inner k, Introversal);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_MULTIDIM_TRAITS_HPP
#define SAKE_CORE_RANGE_MULTIDIM_TRAITS_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/core/introspection/has_type.hpp>
#include <sake/core/iterator/categories.hpp>
#include <sake/core/range/multidim_traits_fwd.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

template< class R >
struct range_multidim_traits
    : sake::extension::range_multidim_traits<R>
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct range_multidim_traits
    : sake::default_impl::range_multidim_traits<R>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    has_type_outer_range,
    outer_range
)

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    has_type_const_outer_range,
    const_outer_range
)

namespace range_multidim_traits_private
{

template< class R, class Outer >
struct impl
{
    typedef boost::true_type enable_tag;

    typedef Outer outer_range;

    static outer_range
    outer(R& r)
    { return r.outer(); }

    template< class OuterJ, class InnerK, class Introversal >
    static typename sake::range_iterator< R, Introversal >::type
    iter_at(R& r, OuterJ const & j, InnerK const & k, Introversal)
    { return r.iter_at(j, k, Introversal()); }
};

template< class R, bool = has_type_outer_range<R>::value >
struct dispatch;

template< class R, bool = has_type_const_outer_range<R>::value >
struct const_dispatch;

template< class R >
struct dispatch< R, false >
    : range_multidim_traits_private::const_dispatch<R>
{ };

template< class R >
struct dispatch< R, true >
    : range_multidim_traits_private::impl<
          R, typename R::outer_range >
{ };

template< class R >
struct const_dispatch< R, false >
{ typedef boost::false_type enable_tag; };

template< class R >
struct const_dispatch< R, true >
    : range_multidim_traits_private::impl<
          R const, typename R::const_outer_range >
{ };

} // namespace range_multidim_traits_private

template< class R >
struct range_multidim_traits
    : sake::default_impl::range_multidim_traits_private::dispatch<R>
{ };

template< class R >
struct range_multidim_traits< R const >
    : sake::default_impl::range_multidim_traits_private::const_dispatch<R>
{ };

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_MULTIDIM_TRAITS_HPP
