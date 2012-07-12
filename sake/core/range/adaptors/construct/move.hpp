/*******************************************************************************
 * sake/core/range/adaptors/construct/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::adaptors::construct::move(R&& r)
 *     -> range::adaptors::construct::result_of::move<R>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPTORS_CONSTRUCT_MOVE_HPP
#define SAKE_CORE_RANGE_ADAPTORS_CONSTRUCT_MOVE_HPP

#include <boost/config.hpp>

#include <sake/core/config.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adaptors/construct/fwd.hpp>
#include <sake/core/range/adaptors/fwd.hpp>
#include <sake/core/range/adaptors/move.hpp>
#include <sake/core/range/adaptors/traits/base.hpp>
#include <sake/core/range/adaptors/traits/enable.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

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

template< class R >
struct move
{ typedef sake::range::adaptors::move<
    typename sake::range::adaptors::traits::base<R>::type > type; };

} // namespace result_of

namespace functional
{

struct move
{
    SAKE_RESULT_FROM_METAFUNCTION(
        sake::range::adaptors::construct::result_of::move, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< move ( R ) >::type
    operator()(R&& r) const
    { return apply_impl(sake::forward<R>(r)); }
    
#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::adaptors::traits::lazy_val_enabler< R,
        result< move ( R ) > >::type
    operator()(R r) const
    { return apply_impl(sake::move(r)); }

    template< class R >
    typename sake::range::adaptors::traits::lazy_ref_enabler< R,
        result< move ( R& ) > >::type
    operator()(R& r) const
    { return apply_impl(r); }

    template< class R >
    typename sake::range::adaptors::traits::lazy_ref_enabler< R,
        result< move ( R const & ) > >::type
    operator()(R const & r) const
    { return apply_impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    template< class R >
    static typename result< move ( SAKE_FWD2_PARAM( R ) ) >::type
    apply_impl(SAKE_FWD2_REF( R ) r)
    {
        return typename result<
            move ( SAKE_FWD2_PARAM( R ) )
        >::type(sake::forward<R>(r));
    }
};

} // namespace functional

#ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
namespace move_adl_barrier
{ sake::range::adaptors::construct::functional::move const move = { }; }
using namespace move_adl_barrier;
#else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
sake::range::adaptors::construct::functional::move const move = { };
#endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS

} // namespace construct

} // namespace adaptors

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPTORS_CONSTRUCT_MOVE_HPP
