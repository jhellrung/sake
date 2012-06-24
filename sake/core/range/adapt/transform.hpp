/*******************************************************************************
 * sake/core/range/adapt/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::adapt::transform(R&& r, F f)
 *     -> range::adapt::result_of::transform<R,F>::type
 * struct range::adapt::functional::transform
 *
 * r | range::adapt::transform(f)
 *     -> range::adapt::result_of::transform<R,F>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_MOVE_HPP
#define SAKE_CORE_RANGE_ADAPT_MOVE_HPP

#include <boost/config.hpp>
#include <boost/mpl/map/map0.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/memberwise/default_constructor.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/traits/base.hpp>
#include <sake/core/range/adapt/traits/enable.hpp>
#include <sake/core/range/adaptors/transform.hpp>
#include <sake/core/utility/base_member.hpp>
#include <sake/core/utility/call_traits.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

namespace result_of
{

template< class R, class F = void, class Params = boost::mpl::map0<> >
struct transform
{
    typedef sake::range::adaptors::transform<
        typename sake::range::adapt::traits::base<R>::type,
        typename boost_ext::remove_qualifiers<F>::type,
        Params
    > type;
};

template< class F, class Params >
struct transform< F, void, Params >
{
    typedef sake::range::adapt::functional::transform<
        typename boost_ext::remove_qualifiers<F>::type,
        Params
    > type;
};

} // namespace result_of

namespace functional
{

template< class F /*= void */, class Params /*= boost::mpl::map0<>*/ >
struct transform
    : base_member<F>
{
    BOOST_STATIC_ASSERT((!boost::is_void<F>::value));

    SAKE_MEMBERWISE_DEFAULT_CONSTRUCTOR(
        typename transform,
        (( base_member<F> ))
    )

    explicit transform(typename sake::call_traits<F>::param_type f)
        : base_member<F>(f)
    { }

    typename boost_ext::add_reference<F>::type
    function()
    { return base_member<F>::member(); }
    typename boost_ext::add_reference_add_const<F>::type
    function() const
    { return base_member<F>::member(); }

    template< class > struct result;
    template< class This, class R >
    struct result< This ( R ) >
        : sake::range::adapt::result_of::transform< R, F, Params >
    { };

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< transform ( R ) >::type
    operator()(R&& r) const
    {
        return typename result< transform ( R ) >::type(
            sake::forward<R>(r), function());
    }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::adapt::traits::lazy_val_enabler<
        R, result< transform ( R ) > >::type
    operator()(R r) const
    {
        return typename result< transform ( R ) >::type(
            sake::move(r), function());
    }

    template< class R >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R& ) > >::type
    operator()(R& r) const
    { return typename result< transform ( R& ) >::type(r, function()); }

    template< class R >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R const & ) > >::type
    operator()(R const & r) const
    { return typename result< transform ( R const & ) >::type(r, function()); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

template<>
struct transform< void, boost::mpl::map0<> >
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::adapt::result_of::transform, (1,2) )

    template< class F >
    typename result< transform ( F ) >::type
    operator()(F const & f) const
    { return typename result< transform ( F ) >::type(f); }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename result< transform ( R, F ) >::type
    operator()(R&& r, F const & f) const
    {
        return typename result< transform ( R, F ) >::type(
            sake::forward<R>(r), f);
    }
    
#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename sake::range::adapt::traits::lazy_val_enabler<
        R, result< transform ( R, F ) > >::type
    operator()(R r, F const & f) const
    { return typename result< transform ( R, F ) >::type(sake::move(r), f); }

    template< class R, class F >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R&, F ) > >::type
    operator()(R& r, F const & f) const
    { return typename result< transform ( R&, F ) >::type(r,f); }

    template< class R, class F >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< transform ( R const &, F ) > >::type
    operator()(R const & r, F const & f) const
    { return typename result< transform ( R const &, F ) >::type(r,f); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

} // namespace functional

static sake::range::adapt::functional::transform<> const transform = { };

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F >
inline typename sake::range::adapt::result_of::transform<R,F>::type
operator|(R&& r, sake::range::adapt::functional::transform<F> const & f)
{
    return typename sake::range::adapt::
        result_of::transform<R,F>::type(sake::forward<R>(r), f.function());
}

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class R, class F >
inline typename sake::range::adapt::traits::lazy_val_enabler<
    R, sake::range::adapt::result_of::transform<R,F> >::type
operator|(R r, sake::range::adapt::functional::transform<F> const & f)
{
    return typename sake::range::adapt::
        result_of::transform<R>::type(sake::move(r), f.function());
}

template< class R, class F >
inline typename sake::range::adapt::traits::lazy_ref_enabler<
    R, sake::range::adapt::result_of::transform< R&, F > >::type
operator|(R& r, sake::range::adapt::functional::transform<F> const & f)
{
    return typename sake::range::adapt::
        result_of::transform< R& >::type(r, f.function());
}

template< class R, class F >
inline typename sake::range::adapt::traits::lazy_ref_enabler<
    R, sake::range::adapt::result_of::transform< R const &, F > >::type
operator|(R const & r, sake::range::adapt::functional::transform<F> const & f)
{
    return typename sake::range::adapt::
        result_of::transform< R const & >::type(r, f.function());
}

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_MOVE_HPP
