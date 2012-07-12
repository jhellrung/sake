/*******************************************************************************
 * sake/core/range/adaptors/construct/transform.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::adaptors::construct::transform(R&& r, F f)
 *     -> range::adaptors::construct::result_of::transform<R,F>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_CONSTRUCT_TRANSFORM_HPP
#define SAKE_CORE_RANGE_ADAPTORS_CONSTRUCT_TRANSFORM_HPP

#include <boost/concept/assert.hpp>
#include <boost/config.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/map/map0.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/concepts/Function.hpp>
#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adaptors/construct/fwd.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/traits/base.hpp>
#include <sake/core/range/adaptors/traits/enable.hpp>
#include <sake/core/range/adaptors/transform.hpp>
#include <sake/core/range/concepts/fwd.hpp>
#include <sake/core/range/concepts/Range.hpp>
#include <sake/core/range/traits.hpp>
#include <sake/core/range/traits_fwd.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

namespace range
{

namespace adaptors
{

namespace construct
{

namespace result_of
{

template< class R, class F, class Params /*= boost::mpl::map0<>*/ >
struct transform
{
private:
    typedef typename boost_ext::remove_reference<R>::type range_type;
    typedef typename boost_ext::remove_qualifiers<F>::type function_type;
    typedef typename sake::remove_type_tag<
        typename boost_ext::remove_qualifiers< Params >::type
    >::type params_type;
    BOOST_CONCEPT_ASSERT((sake::concepts::Range< range_type >));
    BOOST_CONCEPT_ASSERT((sake::concepts::Function<
        function_type,
        void ( typename sake::range_reference< range_type >::type )
    >));
    BOOST_STATIC_ASSERT((boost::mpl::is_sequence< params_type >::value));
public:
    typedef sake::range::adaptors::transform<
        typename sake::range::adaptors::traits::base<R>::type,
        function_type,
        params_type
    > type;
};

} // namespace result_of

namespace functional
{

struct transform
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::adaptors::construct::result_of::transform, (2,3) )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename result< transform ( R, F ) >::type
    operator()(R&& r, F const & f) const
    { return apply_impl(sake::forward<R>(r), f); }
    template< class R, class F, class Params >
    typename result< transform ( R, F, Params ) >::type
    operator()(R&& r, F const & f, Params) const
    { return apply_impl(sake::forward<R>(r), f, Params()); }
    
#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R, class F >
    typename sake::range::adaptors::traits::lazy_val_enabler< R,
        result< transform ( R, F ) > >::type
    operator()(R r, F const & f) const
    { return apply_impl(sake::move(r), f); }
    template< class R, class F, class Params >
    typename sake::range::adaptors::traits::lazy_val_enabler< R,
        result< transform ( R, F, Params ) > >::type
    operator()(R r, F const & f, Params) const
    { return apply_impl(sake::move(r), f, Params()); }

    template< class R, class F >
    typename sake::range::adaptors::traits::lazy_ref_enabler< R,
        result< transform ( R&, F ) > >::type
    operator()(R& r, F const & f) const
    { return apply_impl(r,f); }
    template< class R, class F, class Params >
    typename sake::range::adaptors::traits::lazy_ref_enabler< R,
        result< transform ( R&, F, Params ) > >::type
    operator()(R& r, F const & f, Params) const
    { return apply_impl(r, f, Params()); }

    template< class R, class F >
    typename sake::range::adaptors::traits::lazy_ref_enabler< R,
        result< transform ( R const &, F ) > >::type
    operator()(R const & r, F const & f) const
    { return apply_impl(r,f); }
    template< class R, class F, class Params >
    typename sake::range::adaptors::traits::lazy_ref_enabler< R,
        result< transform ( R const &, F, Params ) > >::type
    operator()(R const & r, F const & f, Params) const
    { return apply_impl(r, f, Params()); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class R, class F >
    static typename result<
        transform ( SAKE_FWD2_PARAM( R ), F ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r, F const & f)
    {
        return typename result<
            transform ( SAKE_FWD2_PARAM( R ), F )
        >::type(sake::forward<R>(r), f);
    }

    template< class R, class F, class Params >
    static typename result<
        transform ( SAKE_FWD2_PARAM( R ), F, Params ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r, F const & f, Params)
    {
        return typename result<
            transform ( SAKE_FWD2_PARAM( R ), F, Params )
        >::type(sake::forward<R>(r), f);
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace transform_adl_barrier
{ sake::range::adaptors::construct::functional::transform const transform = { }; }
using namespace transform_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::adaptors::construct::functional::transform const transform = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace construct

} // namespace adaptors

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_CONSTRUCT_TRANSFORM_HPP
