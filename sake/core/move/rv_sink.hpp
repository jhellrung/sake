/*******************************************************************************
 * sake/core/move/rv_sink.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct rv_sink<
 *     Visitor,
 *     Result = default_tag,
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 *
 * rv_sink, when used as a function parameter, allows capturing of arbitrary
 * rvalues of types with rvalue reference emulation.
 *
 * Example:
 *
 * struct f
 * {
 *     typedef ... result_type;
 *     typedef rv_sink< f, result_type > rv_sink_type;
 *
 *     // lvalues
 *     template< class T >
 *     result_type
 *     operator()(T& x) const
 *     { ... }
 *
 *     // const lvalues + non-movable rvalues
 *     template< class T >
 *     typename boost::disable_if<
 *         boost_ext::is_convertible< T&, rv_sink_type >,
 *         result_type
 *     >::type
 *     operator()(T const & x) const
 *     { ... }
 *
 *     // movable rvalues
 *     result_type
 *     operator()(rv_sink_type const x) const
 *     { return x(*this); }
 * };
 ******************************************************************************/

#ifndef SAKE_CORE_MOVE_RV_SINK_HPP
#define SAKE_CORE_MOVE_RV_SINK_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_RVALUE_REFERENCES

#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/result_type.hpp>

#include <sake/core/introspection/has_type_result_type.hpp>
#include <sake/core/utility/default_tag.hpp>

namespace sake
{

template<
    class Visitor,
    class Result = sake::default_tag,
    class Pred = boost::mpl::always< boost::true_type >,
    bool Enable = true
>
struct rv_sink
{
    BOOST_STATIC_ASSERT((Enable));

    typedef typename sake::lazy_replace_default_tag<
        Result,
        boost::mpl::eval_if<
            sake::has_type_result_type< Visitor >,
            boost_ext::mpl::result_type< Visitor >,
            boost::mpl::identity< void >
        >
    >::type result_type;

    template< class T >
    explicit rv_sink(T* const p)
        : m_apply(apply<T>),
          mp(static_cast< void* >(p))
    { BOOST_STATIC_ASSERT((boost::mpl::apply1< Pred, T >::type::value)); }

    result_type operator()(Visitor visitor) const
    { return m_apply(visitor, mp); }

private:
    template< class T >
    static result_type apply(Visitor visitor, void* p)
    { return visitor(sake::move(*static_cast< T* >(p))); }

    result_type (&m_apply)(Visitor, void*);
    void* const mp;
};

namespace rv_sink_private
{

// These helper structs are necessary to prevent MSVC9 from ICE'ing on
// expressions like
//
// template< class V, class R, class P >
// operator rv_sink< V,R,P, boost::mpl::apply1< Pred, T >::type::value >()
// { return rv_sink< V,R,P, boost::mpl::apply1< Pred, T >::type::value >(this); }
//
// Instead, use
//
// template< class V, class R, class P >
// operator rv_sink< V,R,P, apply1_pred< Pred, T >::value >()
// { return rv_sink< V,R,P, apply1_pred< Pred, T >::value >(this); }

template< class Pred, class T >
struct apply1_pred
{ static bool const value = boost::mpl::apply1< Pred, T >::type::value; };

template< bool C, class Pred, class T >
struct apply1_pred_if_c;

template< class Pred, class T >
struct apply1_pred_if_c< true, Pred, T >
    : apply1_pred< Pred, T >
{ };

template< class Pred, class T >
struct apply1_pred_if_c< false, Pred, T >
{ static bool const value = false; };

} // namespace rv_sink_private

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_RV_SINK_HPP
