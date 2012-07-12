/*******************************************************************************
 * sake/core/range/adaptors/multidim/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_MULTIDIM_MOVE_HPP
#define SAKE_CORE_RANGE_ADAPTORS_MULTIDIM_MOVE_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/propagate_const.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/range/adaptors/construct/fwd.hpp>
#include <sake/core/range/adaptors/construct/move.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/transform.hpp>
#include <sake/core/range/multidim_traits_fwd.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

namespace extension
{

namespace range_multidim_traits_adaptors_move_private
{

template< class This >
struct enabler
    : boost::enable_if_c< sake::range_multidim_enable<
          typename boost_ext::remove_reference<
              typename boost_ext::propagate_const<
                  This, typename This::base_type >::type >::type
      >::value >
{ };

template< class This >
struct impl
{
    typedef boost::true_type enable_tag;

    typedef typename boost_ext::remove_reference<
        typename boost_ext::propagate_const<
            This, typename This::base_type >::type
    >::type base_;
   
    typedef sake::range_multidim_traits< base_ > base_traits;

    typedef sake::range::adaptors::transform<
        typename base_traits::outer_range,
        sake::range::adaptors::construct::functional::move
    > outer_range;

    static outer_range
    outer(This& r)
    { return outer_range(base_traits::outer(r.base())); }

    template< class Outer, class Inner, class Introterminal >
    static typename sake::range_iterator< This, Introterminal >::type
    iter_at(This& r, Outer const & j, Inner const & k, Introterminal)
    {
        return typename sake::range_iterator< This, Introterminal >::type(
            base_traits::iter_at(r.base(), j.base(), k.base(), Introterminal()));
    }
};

} // namespace range_multidim_traits_adaptors_move_private

template< class R >
struct range_multidim_traits<
    sake::range::adaptors::move<R>,
    typename range_multidim_traits_adaptors_move_private::enabler<
        sake::range::adaptors::move<R> >::type
>
    : range_multidim_traits_adaptors_move_private::impl<
          sake::range::adaptors::move<R> >
{ };

template< class R >
struct range_multidim_traits<
    sake::range::adaptors::move<R> const,
    typename range_multidim_traits_adaptors_move_private::enabler<
        sake::range::adaptors::move<R> const >::type
>
    : range_multidim_traits_adaptors_move_private::impl<
          sake::range::adaptors::move<R> const >
{ };

} // namespace extension

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_MULTIDIM_MOVE_HPP
