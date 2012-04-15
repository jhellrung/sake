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
#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/add_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/is_same_sans_qualifiers.hpp>

#include <sake/core/move/forward.hpp>
#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv_sink.hpp>
#include <sake/core/utility/call_traits.hpp>
#include <sake/core/utility/default_tag.hpp>
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
template< class T, class U >
struct enable_clv;

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
    typedef typename rv_sink_traits::rv_param<T>::type rv_param_type;
    typedef typename construct_private::rv_sink<T>::type rv_sink_type;
public:
    // lvalues
    template< class U >
    T
    operator()(U& x) const
    { return static_cast<T>(x); }
    // T rvalues
    T
    operator()(rv_param_type x) const
    { return static_cast<T>(x); }
    // movable rvalues
    T
    operator()(rv_sink_type x) const
    { return x(); }
    // const lvalues + non-movable rvalues
    template< class U >
    typename construct_private::enable_clv<T,U>::type
    operator()(U const & x) const
    { return static_cast<T>(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_RESULT_TYPE \
    T
#define SAKE_OVERLOAD_FUNCTION_NAME \
    operator()
#define SAKE_OVERLOAD_DECLARATION_SUFFIX \
    const
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
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
{ return sake::functional::construct<T>()(sake::forward<U>(x)...); }

#else // #if !defined( ... ) && !defined( ... )

template< class T >
inline T
construct()
{ return sake::functional::construct<T>()(); }

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline T
construct(U&& x)
{ return sake::functional::construct<T>()(sake::forward<U>(x)); }

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

template< class T, class U >
inline T
construct(U& x)
{ return sake::functional::construct<T>()(x); }

template< class T >
inline T
construct(typename rv_sink_traits::rv_param<T>::type x)
{ return sake::functional::construct<T>()(x); }

template< class T >
inline T
construct(typename construct_private::rv_sink<T>::type x)
{ return x(sake::functional::construct<T>()); }

template< class T, class U >
inline typename construct_private::enable_clv<T,U>::type
construct(U const & x)
{ return sake::functional::construct<T>()(x); }

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, n, classT_tuple ) \
    class T, BOOST_PP_TUPLE_REM_CTOR( n, classT_tuple )
#define SAKE_OVERLOAD_RESULT_TYPE \
    T
#define SAKE_OVERLOAD_FUNCTION_NAME \
    construct
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return sake::functional::construct<T>() forward_x_tuple ;
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
    typedef sake::rv_sink<
        sake::functional::construct<T>, // Visitor
        sake::default_tag, // Result
        boost::mpl::not_< boost::is_same< T, boost::mpl::_1 > > // Pred
    > type;
};

template< class T, class U >
struct enable_clv
    : boost::disable_if_c<
          boost_ext::mpl::or2<
              boost_ext::is_same_sans_qualifiers<
                  U, typename rv_sink_traits::rv_param<T>::type >,
              sake::is_movable<U>
          >::value,
          T
      >
{ };

} // namespace construct_private

} // namespace sake

#endif // #ifndef SAKE_CORE_FUNCTIONAL_CONSTRUCT_HPP
