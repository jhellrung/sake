/*******************************************************************************
 * sake/core/functional/construct.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct functional::construct<T>
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP
#define SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/static_assert.hpp>

#include <sake/boost_ext/type_traits/is_cv_or.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/move.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/overload.hpp>

#ifndef SAKE_CONSTRUCT_PERFECT_MAX_ARITY
#define SAKE_CONSTRUCT_PERFECT_MAX_ARITY 4
#endif // #ifndef SAKE_CONSTRUCT_PERFECT_MAX_ARITY

#ifndef SAKE_CONSTRUCT_FWD2_MAX_ARITY
#define SAKE_CONSTRUCT_FWD2_MAX_ARITY 6
#endif // #ifndef SAKE_CONSTRUCT_FWD2_MAX_ARITY

#ifndef SAKE_CONSTRUCT_FWD_MAX_ARITY
#define SAKE_CONSTRUCT_FWD_MAX_ARITY 8
#endif // #ifndef SAKE_CONSTRUCT_FWD_MAX_ARITY

#define SAKE_CONSTRUCT_MAX_ARITY SAKE_CONSTRUCT_PERFECT_MAX_ARITY
#if SAKE_CONSTRUCT_MAX_ARITY < SAKE_CONSTRUCT_FWD2_MAX_ARITY
#undef SAKE_CONSTRUCT_MAX_ARITY
#define SAKE_CONSTRUCT_MAX_ARITY SAKE_CONSTRUCT_FWD2_MAX_ARITY
#endif // #if SAKE_CONSTRUCT_MAX_ARITY < SAKE_CONSTRUCT_FWD2_MAX_ARITY
#if SAKE_CONSTRUCT_MAX_ARITY < SAKE_CONSTRUCT_FWD_MAX_ARITY
#undef SAKE_CONSTRUCT_MAX_ARITY
#define SAKE_CONSTRUCT_MAX_ARITY SAKE_CONSTRUCT_FWD_MAX_ARITY
#endif // #if SAKE_CONSTRUCT_MAX_ARITY < SAKE_CONSTRUCT_FWD_MAX_ARITY

namespace sake
{

namespace functional
{

template< class T >
struct construct
{
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));

    typedef T result_type;

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

    template< class... U >
    T operator()(U&&... x) const
    { return T(sake::forward<U>(x)...); }

#else // #if !defined( ... ) && !defined( ... )

    T operator()() const
    { return T(); }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    T operator()(U&& x) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef sake::rv_sink_traits1<T> rv_sink_traits_;
    typedef typename rv_sink_traits_::template
        default_< construct > rv_sink_default_type;
public:
    // lvalues + movable explicit rvalues
    template< class U >
    T operator()(U& x) const
    { return static_cast<T>(x); }
    // T rvalues
    T operator()(typename rv_sink_traits_::primary_type x) const
    { return sake::move(x.value); }
    // movable implicit rvalues
    T operator()(rv_sink_default_type x) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U >
    typename rv_sink_traits_::template cref_enabler<U,T>::type
    operator()(U const & x) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_RESULT_TYPE \
    T
#define SAKE_OVERLOAD_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, x_tuple, forward_x_tuple ) \
    return T forward_x_tuple ;
#define SAKE_OVERLOAD_MIN_ARITY         2
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_CONSTRUCT_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_CONSTRUCT_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_CONSTRUCT_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && !defined( ... )

};

template<>
struct construct< void >
{
    typedef void result_type;

#ifndef BOOST_NO_VARIADIC_TEMPLATES

    template< class... U >
    void operator()(U const &...) const
    { }

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

    void operator()() const { }

#define SAKE_OVERLOAD_RESULT_TYPE \
    void
#define SAKE_OVERLOAD_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple )
#define SAKE_OVERLOAD_MIN_ARITY     1
#define SAKE_OVERLOAD_FWD_MAX_ARITY SAKE_CONSTRUCT_FWD_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

};

} // namespace functional

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP
