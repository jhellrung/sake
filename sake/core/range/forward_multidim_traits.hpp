/*******************************************************************************
 * sake/core/range/forward_multidim_traits.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct range_forward_multidim_traits<R>
 * struct extension::range_forward_multidim_traits< R, Enable = void >
 * struct default_impl::range_forward_multidim_traits<R>
 *
 * struct range_forward_multidim_enable<R>
 * struct range_forward_multidim_outer<R>
 *
 * range_forward_multidim_traits has the following interface.
 *
 * template< class R >
 * struct range_forward_multidim_traits
 * {
 *     typedef ... enable_tag;
 *
 *     // Only if enable_tag::value == true:
 *
 *     typedef ... outer_range;
 *
 *     static outer_range
 *     outer(R&& r);
 *
 *     template< class Outer, class Inner, class Introversal >
 *     static typename range_iterator< R, Introversal >::type
 *     iter_at(R&& r, Outer j, Inner k, Introversal);
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_FORWARD_MULTIDIM_TRAITS_HPP
#define SAKE_CORE_RANGE_FORWARD_MULTIDIM_TRAITS_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/move/as_lvalue.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/transform.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/move.hpp>
#include <sake/core/range/forward_multidim_traits_fwd.hpp>
#include <sake/core/range/forward_traits.hpp>
#include <sake/core/range/forward_traits_fwd.hpp>
#include <sake/core/range/multidim_traits.hpp>
#include <sake/core/range/multidim_traits_fwd.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>

namespace sake
{

namespace range_forward_multidim_traits_private
{

template< class R >
struct dispatch0_bool
{
    static bool const value = sake::range_multidim_enable<
        typename boost_ext::remove_reference<R>::type >::value;
};

template< class R >
struct dispatch1_bool
{
    static bool const value = !boost::is_same<
        typename sake::range_iterator<R>::type,
        typename sake::range_forward_iterator<R>::type
    >::value;
};

template< class R, bool = dispatch0_bool<R>::value >
struct dispatch0;

template< class R, bool = dispatch1_bool<R>::value >
struct dispatch1;

template< class R >
struct dispatch0< R, false >
{ typedef boost::false_type enable_tag; };

template< class R >
struct dispatch0< R, true >
    : dispatch1<R>
{ };

template< class R >
struct dispatch1< R, false >
{
    typedef boost::true_type enable_tag;
    typedef sake::range_multidim_traits<R> base_traits;
    typedef typename base_traits::outer_range base_outer_range;

private:
    typedef typename boost_ext::add_reference<
        typename boost_ext::add_rvalue_reference<R>::type >::type param_type;
public:

    static base_outer_range
    outer(param_type r)
    { return base_traits::outer(SAKE_AS_LVALUE(r)); }

    template< class Outer, class Inner >
    static typename sake::range_iterator<R>::type
    iter_at(param_type r, Outer const & j, Inner const & k)
    { return base_traits::iter_at(SAKE_AS_LVALUE(r), j, k); }

    template< class Outer, class Inner, class Introversal >
    static typename sake::range_iterator< R, Introversal >::type
    iter_at(param_type r, Outer const & j, Inner const & k, Introversal)
    {
        BOOST_STATIC_ASSERT((boost::is_same<
            typename sake::range_iterator< R, Introversal >::type,
            typename sake::range_forward_iterator< R, Introversal >::type
        >::value));
        return base_traits::iter_at(SAKE_AS_LVALUE(r), j, k, Introversal());
    }
};

template< class R >
struct dispatch1< R, true >
{
    typedef boost::true_type enable_tag;
    typedef sake::range_multidim_traits<R> base_traits;
    typedef typename base_traits::outer_range base_outer_range;

    typedef sake::range::adaptors::transform<
        base_outer_range,
        sake::range::adapt::functional::move
    > outer_range;

private:
    typedef typename boost_ext::add_reference<
        typename boost_ext::add_rvalue_reference<R>::type >::type param_type;
public:

    static outer_range
    outer(param_type r)
    { return outer_range(base_traits::outer(SAKE_AS_LVALUE(r))); }

    template< class Outer, class Inner >
    static typename sake::range_forward_iterator<R>::type
    iter_at(param_type r, Outer const & j, Inner const & k)
    {
        return typename sake::range_forward_iterator<R>::type(
            base_traits::iter_at(SAKE_AS_LVALUE(r), j.base(), k.base()));
    }

    template< class Outer, class Inner, class Introversal >
    static typename sake::range_forward_iterator< R, Introversal >::type
    iter_at(param_type r, Outer const & j, Inner const & k, Introversal)
    {
        return typename sake::range_forward_iterator< R, Introversal >::type(
            base_traits::iter_at(
                SAKE_AS_LVALUE(r), j.base(), k.base(), Introversal()));
    }
};

template< class R >
struct dispatch0< R&, true >
    : sake::range_multidim_traits<R>
{
    typedef sake::range_multidim_traits<R> base_traits;
    typedef typename base_traits::outer_range base_outer_range;
};

} // namespace range_forward_multidim_traits_private

template< class R >
struct range_forward_multidim_traits
    : range_forward_multidim_traits_private::dispatch0<
          typename boost_ext::remove_rvalue_reference<R>::type >
{ };

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_FORWARD_MULTIDIM_TRAITS_HPP
