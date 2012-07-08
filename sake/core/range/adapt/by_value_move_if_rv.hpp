/*******************************************************************************
 * sake/core/range/adapt/by_value_move_if_rv.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_RANGE_ADAPT_BY_VALUE_IF_RV( r )
 ******************************************************************************/

#ifndef SAKE_CORE_RANGE_ADAPT_BY_VALUE_MOVE_IF_RV_HPP
#define SAKE_CORE_RANGE_ADAPT_BY_VALUE_MOVE_IF_RV_HPP

#include <boost/config.hpp>

#ifndef BOOST_NO_RVALUE_REFERENCES

#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_lvalue_reference_to_nonconst.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/emplacer/construct.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>
#include <sake/core/range/adaptors/by_value.hpp>
#include <sake/core/range/adaptors/move.hpp>
#include <sake/core/range/traits.hpp>

#define SAKE_RANGE_ADAPT_BY_VALUE_IF_RV( r ) \
    ::sake::range::adapt::by_value_move_if_rv(r)

namespace sake
{

namespace range
{

namespace adapt
{

namespace by_value_move_if_rv_private
{

template< class R >
struct dispatch_index
{
    static bool const by_value_value = !sake::range_is_adapt_by_value<R>::value;
    typedef typename sake::range_reference<
        typename boost_ext::remove_reference<R>::type >::type reference;
    static bool const move_value =
        boost_ext::is_lvalue_reference_to_nonconst< reference >::value
    static int const value = 2 * by_value_value + move_value;
};

template< class R, int = dispatch_index<R>::value >
struct dispatch;

template< class R >
struct dispatch<R,3>
{
    typedef sake::range::adaptors::by_value<
        sake::range::adaptors::move<R> > type;
    static type apply(R&& r) const
    { return type(sake::construct::emplacer(sake::move(r))); }
};

template< class R >
struct dispatch<R,2>
{
    typedef sake::range::adaptors::by_value<R> type;
    static type apply(R&& r) const
    { return type(sake::move(r)); }
};

template< class R >
struct dispatch<R,1>
{
    typedef sake::range::adaptors::move<R> type;
    static type apply(R&& r) const
    { return type(sake::forward<R>(r)); }
};

template< class R >
struct dispatch<R,0>
{
    typedef R&& type;
    static type apply(R&& r) const
    { return sake::forward<R>(r); }
};

} // namespace by_value_move_if_rv_private

template< class R >
inline typename by_value_move_if_rv_private::dispatch<R>::type
by_value_move_if_rv(R&& r)
{ return by_value_move_if_rv_private::dispatch<R>::apply(sake::forward<R>(r)); }

} // namespace adapt

} // namespace range

} // namespace sake

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_rvalue_reference.hpp>

#include <sake/core/expr_traits/type_tag_of.hpp>
#include <sake/core/move/has_move_emulation.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/range/is_adapt_by_value.hpp>
#include <sake/core/utility/type_tag.hpp>

#define SAKE_RANGE_ADAPT_BY_VALUE_IF_RV( r ) \
    ::sake::range::adapt::by_value_move_if_rv_private:: \
        dispatch_(SAKE_EXPR_TYPE_TAG_OF( r ))(r)

namespace sake
{

namespace range
{

namespace adapt
{

namespace by_value_move_if_rv_private
{

template< class R >
struct dispatch_index
{
    static bool const by_value_value = !sake::range_is_adapt_by_value<R>::value
    typedef typename sake::range_reference<
        typename boost_ext::remove_reference<R>::type >::type reference;
    static bool const move_value = boost_ext::mpl::and2<
        boost_ext::is_lvalue_reference_to_nonconst< reference >,
        sake::has_move_emulation<
            typename boost_ext::remove_reference< reference >::type >
    >::value;
    static int const value = 2 * by_value_value + move_value;
};

template< class R, int = dispatch_index<R>::value >
struct dispatch;

template< class R >
struct dispatch<R,3>
{
    BOOST_STATIC_ASSERT((sake::has_move_emulation<R>::value));
    typedef sake::range::adaptors::by_value<
        sake::range::adaptors::move<R> > type;
    type operator()(SAKE_RV_REF( R ) r) const
    { return type(sake::construct::emplacer(r)); }
};

template< class R >
struct dispatch<R,2>
{
    BOOST_STATIC_ASSERT((sake::has_move_emulation<R>::value));
    typedef sake::range::adaptors::by_value<R> type;
    type operator()(SAKE_RV_REF( R ) r) const
    { return type(r); }
};

template< class R >
struct dispatch<R,1>
{
    typedef typename boost_ext::add_reference_add_const<
        typename boost_ext::add_rvalue_reference<R>::type >::type param_type;
    typedef sake::range::adaptors::move<R> type;
    type operator()(param_type r) const
    { return type(r); }
};

template< class R >
struct dispatch<R,0>
{
    typedef typename boost_ext::add_reference_add_const<
        typename boost_ext::add_rvalue_reference<R>::type >::type param_type;
    typedef param_type type;
    type operator()(param_type r) const
    { return r; }
};

template< class R >
inline dispatch< typename boost_ext::remove_rvalue_reference<R>::type >
dispatch_(sake::type_tag<R>)
{ return dispatch< typename boost_ext::remove_rvalue_reference<R>::type >(); }

} // namespace by_value_move_if_rv_private

} // namespace adapt

} // namespace range

} // namespace sake

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_RANGE_ADAPT_BY_VALUE_MOVE_IF_RV_HPP
