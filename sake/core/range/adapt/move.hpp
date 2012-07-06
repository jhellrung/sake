/*******************************************************************************
 * sake/core/range/adapt/move.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * range::adapt::move(R&& r)
 *     -> range::adapt::result_of::move<R>::type
 * struct range::adapt::functional::move
 *
 * r | range::adapt::move
 *     -> range::adapt::result_of::move<R>::type
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_MOVE_HPP
#define SAKE_CORE_RANGE_ADAPT_MOVE_HPP

#include <boost/config.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/adapt/fwd.hpp>
#include <sake/core/range/adapt/traits/base.hpp>
#include <sake/core/range/adapt/traits/enable.hpp>
#include <sake/core/range/adaptors/move.hpp>
#include <sake/core/utility/result_from_metafunction.hpp>

namespace sake
{

namespace range
{

namespace adapt
{

/*******************************************************************************
 * struct range::adapt::result_of::move<R>
 ******************************************************************************/

namespace result_of
{

template< class R >
struct move
{
    typedef sake::range::adaptors::move<
        typename sake::range::adapt::traits::base<R>::type > type;
};

} // namespace result_of

/*******************************************************************************
 * namespace range::adapt::move_private
 ******************************************************************************/

namespace move_private
{

template< class R >
inline typename sake::range::adapt::result_of::move<
    SAKE_FWD2_PARAM( R ) >::type
impl(SAKE_FWD2_REF( R ) r)
{
    return typename sake::range::adapt::result_of::move<
        SAKE_FWD2_PARAM( R ) >::type(sake::forward<R>(r));
}

} // namespace move_private

namespace functional
{

/*******************************************************************************
 * struct range::adapt::functional::move
 ******************************************************************************/

struct move
{
    SAKE_RESULT_FROM_METAFUNCTION( sake::range::adapt::result_of::move, 1 )

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename result< move ( R ) >::type
    operator()(R&& r) const
    { return move_private::impl(sake::forward<R>(r)); }
    
#else // #ifndef BOOST_NO_RVALUE_REFERENCES

    template< class R >
    typename sake::range::adapt::traits::lazy_val_enabler<
        R, result< move ( R ) > >::type
    operator()(R r) const
    { return move_private::impl(sake::move(r)); }

    template< class R >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< move ( R& ) > >::type
    operator()(R& r) const
    { return move_private::impl(r); }

    template< class R >
    typename sake::range::adapt::traits::lazy_ref_enabler<
        R, result< move ( R const & ) > >::type
    operator()(R const & r) const
    { return move_private::impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

};

/*******************************************************************************
 * r | range::adapt::move
 *     -> range::adapt::result_of::move<R>::type
 ******************************************************************************/

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class R >
inline typename sake::range::adapt::result_of::move<R>::type
operator|(R&& r, sake::range::adapt::functional::move)
{ return move_private::impl(sake::forward<R>(r)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class R >
inline typename sake::range::adapt::traits::lazy_val_enabler<
    R, sake::range::adapt::result_of::move<R> >::type
operator|(R r, sake::range::adapt::functional::move)
{ return move_private::impl(sake::move(r)); }

template< class R >
inline typename sake::range::adapt::traits::lazy_ref_enabler<
    R, sake::range::adapt::result_of::move< R& > >::type
operator|(R& r, sake::range::adapt::functional::move)
{ return move_private::impl(r); }

template< class R >
inline typename sake::range::adapt::traits::lazy_ref_enabler<
    R, sake::range::adapt::result_of::move< R const & > >::type
operator|(R const & r, sake::range::adapt::functional::move)
{ return move_private::impl(r); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace functional

static sake::range::adapt::functional::move const move = { };

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_MOVE_HPP
