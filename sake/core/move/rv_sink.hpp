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
 * struct rv_sink_traits::rv_param<T>
 *
 * struct rv_sink_visitors::operator_assign<T>
 * rv_sink_visitors::make_operator_assign(T& x)
 *     -> rv_sink_visitors::operator_assign<T>
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
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/result_type.hpp>

#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/introspection/has_type_result_type.hpp>
#include <sake/core/utility/address_of.hpp>
#include <sake/core/utility/default_tag.hpp>

namespace sake
{

namespace rv_sink_private
{

// MSVC9 tends to ICE without this...
template<
    class Pred, class T,
    bool = sake::is_movable<T>::value
>
struct enable_ctor;

} // namespace rv_sink_private

/*******************************************************************************
 * struct rv_sink<
 *     Visitor,
 *     Result = default_tag,
 *     Pred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

template<
    class Visitor,
    class Result = sake::default_tag,
    class Pred = boost::mpl::always< boost::true_type >
>
struct rv_sink
{
    typedef typename sake::lazy_replace_default_tag<
        Result,
        boost::mpl::eval_if<
            sake::has_type_result_type< Visitor >,
            boost_ext::mpl::result_type< Visitor >,
            boost::mpl::identity< void >
        >
    >::type result_type;

    // implicit on purpose
    template< class T >
    rv_sink(T const & x,
        typename rv_sink_private::enable_ctor< Pred, T >::type* = 0)
        : m_apply(apply<T>),
          mp(static_cast< void* >(sake::address_of(const_cast< T& >(x))))
    { }

    result_type operator()() const
    { return m_apply(Visitor(), mp); }

    result_type operator()(Visitor visitor) const
    { return m_apply(visitor, mp); }

private:
    template< class T >
    static result_type apply(Visitor visitor, void* p)
    { return visitor(sake::move(*static_cast< T* >(p))); }

    result_type (&m_apply)(Visitor, void*);
    void* const mp;
};

/*******************************************************************************
 * struct rv_sink_traits::rv_param<T>
 ******************************************************************************/

namespace rv_sink_traits
{

namespace private_
{

template< class T >
struct disabler;

template< class T, bool = sake::is_movable<T>::value >
struct rv_param_dispatch;
template< class T >
struct rv_param_dispatch< T, true >
{ typedef SAKE_RV_REF( T ) type; };
template< class T >
struct rv_param_dispatch< T, false >
{ typedef disabler<T> type; };

} // namespace private_

template< class T >
struct rv_param
    : private_::rv_param_dispatch<T>
{ };

} // namespace rv_sink_traits

/*******************************************************************************
 * struct rv_sink_visitors::operator_assign<T>
 * rv_sink_visitors::make_operator_assign(T& x)
 *     -> rv_sink_visitors::operator_assign<T>
 ******************************************************************************/

namespace rv_sink_visitors
{

template< class T >
struct operator_assign
{
    explicit operator_assign(T& this_) : m_this(this_) { }
    typedef T& result_type;
    template< class U >
    result_type operator()(SAKE_RV_REF( U ) x) const
    { return m_this = x; }
private:
    T& m_this;
};

template< class T >
inline sake::rv_sink_visitors::operator_assign<T>
make_operator_assign(T& x)
{ return sake::rv_sink_visitors::operator_assign<T>(x); }

} // namespace rv_sink_visitors

namespace rv_sink_private
{

// MSVC9 tends to ICE without this...
template< class Pred, class T >
struct enable_ctor< Pred, T, false >
{ };

template< class Pred, class T >
struct enable_ctor< Pred, T, true >
    : boost::enable_if_c< boost::mpl::apply1< Pred, T >::type::value >
{ };

} // namespace rv_sink_private

} // namespace sake

#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

#endif // #ifndef SAKE_CORE_MOVE_RV_SINK_HPP
