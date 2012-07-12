/*******************************************************************************
 * sake/core/range/static_size.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_static_size<R>
 * struct extension::range_static_size< R, Enable = void >
 * struct default_impl::range_static_size<R>
 *
 * struct range_has_static_size< R, Pred >
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_STATIC_SIZE_HPP
#define SAKE_CORE_RANGE_STATIC_SIZE_HPP

#include <cstddef>

#include <boost/mpl/always.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/mpl/if.hpp>

#include <sake/core/introspection/has_isc.hpp>
#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/introspection/has_type.hpp>
#include <sake/core/range/static_size_fwd.hpp>
#include <sake/core/utility/int_tag.hpp>

namespace sake
{

template<
    class R,
    class Pred = boost::mpl::always< boost::true_type >
>
struct range_has_static_size
    : sake::has_isc_value< sake::range_static_size<R>, Pred >
{ };

template< class R >
struct range_static_size
    : sake::extension::range_static_size<
          typename boost::remove_const<R>::type >
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct range_static_size
    : sake::default_impl::range_static_size<R>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_ISC(
    range_has_isc_static_size,
    static_size
)

SAKE_INTROSPECTION_DEFINE_HAS_TYPE(
    range_has_type_static_size_tag,
    static_size_tag
)

namespace range_static_size_private
{

template< class R >
struct dispatch_index
{
    static int const value = boost_ext::mpl::
         if_< sake::default_impl::range_has_isc_static_size<R>,
              sake::int_tag<2> >::type::template
    else_if < sake::default_impl::range_has_type_static_size_tag<R>,
              sake::int_tag<1> >::type::template
    else_   < sake::int_tag<0> >::type::value;
};

template< class R, int = dispatch_index<R>::value >
struct dispatch;

template< class R >
struct dispatch<R,2>
{
    static std::size_t const value = R::static_size;
    typedef dispatch type;
};

template< class R >
struct dispatch<R,1>
    : R::static_size_tag
{ };

template< class R >
struct dispatch<R,0>
{ typedef dispatch type; };

} // namespace range_static_size_private

template< class R >
struct range_static_size
    : range_static_size_private::dispatch<R>
{ };

template< class T, std::size_t N >
struct range_static_size< T[N] >
{
    static std::size_t const value = N;
    typedef range_static_size type;
};

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_STATIC_SIZE_HPP
