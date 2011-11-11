/*******************************************************************************
 * sake/core/functional/construct.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * construct<T>(T0&& x0, ... ) -> T
 * struct functional::construct< T = void >
 ******************************************************************************/

#ifndef SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP
#define SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP

#include <boost/config.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/call_traits.hpp>
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

namespace construct_private
{

template< class T >
struct rv_sink;

} // namespace construct_private

namespace functional
{

template< class T = void >
struct construct
{
    typedef T result_type;

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

    template< class... U >
    T
    operator()(U&&... x) const
    { return T(sake::forward<U>(x)...); }

#else // #if !defined( ... ) && !defined( ... )

    T
    operator()() const
    { return T(); }

#ifndef BOOST_NO_RVALUE_REFERENCES

    template< class U >
    T
    operator()(U&& x) const
    { return static_cast<T>(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

private:
    typedef typename sake::call_traits<
        typename boost_ext::add_rvalue_reference<T>::type
    >::param_type rparam_type;
    typedef typename construct_private::rv_sink<T>::type rv_sink_type;
public:
    // lvalues
    template< class U >
    T
    operator()(U& x) const
    { return static_cast<T>(x); }
    // rvalues to T
    T
    operator()(rparam_type x) const
    { return static_cast<T>(x); }
    // movable rvalues
    T
    operator()(rv_sink_type x) const
    { return x(*this); }
    // const lvalues + non-movable rvalues
    template< class U >
    typename boost::disable_if_c<
        construct_private::disable_cref_cond<T,U>::value,
        T
    >::type
    operator()(U const & x) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_RESULT_TYPE \
    T
#define SAKE_OVERLOAD_CONST_MEMBER_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
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
    void
    operator()(...) const
    { }
};

} // namespace functional

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class T, class... U >
inline T
construct(U&&... x)
{ return functional::construct<T>()(sake::forward<U>(x)...); }

#else // #if !defined( ... ) && !defined( ... )

template< class T >
inline T
construct()
{ return functional::construct<T>()(); }

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline T
construct(U&& x)
{ return functional::construct<T>()(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline T
construct(U& x)
{ return functional::construct<T>()(x); }

template< class T >
inline T
construct(typename sake::call_traits<
    typename boost_ext::add_rvalue_reference<T>::type >::param_type x)
{ return functional::construct<T>()(x); }

template< class T >
inline T
construct(typename construct_private::rv_sink<T>::type x)
{ return x(functional::construct<T>()); }

template< class T, class U >
inline typename boost::disable_if_c<
    construct_private::disable_cref_cond<T,U>::value,
    T
>::type
construct(U const & x)
{ return functional::construct<T>()(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_DECLARE_TMPL_PARAMS( n, classT_tuple ) \
    class T, BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )
#define SAKE_OVERLOAD_RESULT_TYPE \
    T
#define SAKE_OVERLOAD_FUNCTION_NAME \
    construct
#define SAKE_OVERLOAD_BODY( n, T_tuple, x_tuple, forward_x_tuple ) \
    return functional::construct<T>() forward_x_tuple ;
#define SAKE_OVERLOAD_MIN_ARITY         2
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_CONSTRUCT_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_CONSTRUCT_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_CONSTRUCT_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && !defined( ... )

namespace construct_private
{

template< class T >
struct rv_sink
{
    template< class U >
    struct apply
    {
        static bool const value = !boost_ext::is_same_sans_qualifiers<T,U>::value;
        typedef apply type;
    };
    typedef sake::rv_sink< functional::construct<T>, T, rv_sink > type;
};

template< class T, class U >
struct disable_cref_cond
{
    static bool const value =
        boost_ext::is_same_sans_qualifiers<U,T>::value
     || boost_ext::is_convertible< U&, typename rv_sink<T>::type >::value;
    typedef disable_cref_cond type;
};

} // namespace construct_private

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP
