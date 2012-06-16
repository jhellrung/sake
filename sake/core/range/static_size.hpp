/*******************************************************************************
 * sake/core/range/static_size.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range::static_size<R>
 * struct range::extension::static_size< R, Enable = void >
 * struct range::default_impl::static_size<R>
 *
 * struct range::has_static_size< R, Pred >
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_STATIC_SIZE_HPP
#define SAKE_CORE_RANGE_STATIC_SIZE_HPP

#include <cstddef>

#include <boost/mpl/always.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/core/introspection/has_isc.hpp>
#include <sake/core/introspection/has_isc_value.hpp>
#include <sake/core/range/static_size_fwd.hpp>

namespace sake
{

namespace range
{

template<
    class R,
    class Pred = boost::mpl::always< boost::true_type >
>
struct has_static_size
    : sake::has_isc_value< sake::range::static_size<R>, Pred >
{ };

template< class R >
struct static_size
    : sake::range::extension::static_size<
          typename boost::remove_const<R>::type >
{ };

namespace extension
{

template< class R, class Enable /*= void*/ >
struct static_size
    : sake::range::default_impl::static_size<R>
{ };

} // namespace extension

namespace default_impl
{

SAKE_INTROSPECTION_DEFINE_HAS_ISC( has_isc_static_size, static_size )

namespace static_size_private
{

template< class R, bool = has_isc_static_size<R>::value >
struct dispatch;

template< class R >
struct dispatch< R, false >
{ };

template< class R >
struct dispatch< R, true >
{
    static std::size_t const value = R::static_size;
    typedef dispatch type;
};

} // namespace static_size_private

template< class R >
struct static_size
    : static_size_private::dispatch<R>
{ };

} // namespace default_impl

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_STATIC_SIZE_HPP
