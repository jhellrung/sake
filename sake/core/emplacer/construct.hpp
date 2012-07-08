/*******************************************************************************
 * sake/core/emplacer/construct.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * emplacer_construct<T>(U&& y, void* p) -> void
 * emplacer_construct<T>(U&& y) -> T
 *
 * construct::emplacer(U0&& y0, ...) -> emplacer< void ( U0, ... ) >
 * construct::emplacer<T>(U0&& y0, ...) -> emplacer< T ( U0, ... ) >
 *
 * emplacer_construct<T> constructs a T object from the given argument.
 * Note: This is overloaded for sake::emplacer arguments.
 ******************************************************************************/

#ifndef SAKE_CORE_EMPLACER_CONSTRUCT_HPP
#define SAKE_CORE_EMPLACER_CONSTRUCT_HPP

#include <boost/config.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_cv_or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/is_convertible_wnrbt.hpp>
#include <sake/core/utility/overload.hpp>

namespace sake
{

template< class T, class U >
inline void
emplacer_construct(SAKE_FWD2_REF( U ) y, void* const p)
{
    BOOST_STATIC_ASSERT((boost::is_object<T>::value));
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    new(p) T(sake::forward<U>(y));
}

template< class T, class U >
inline T
emplacer_construct(SAKE_FWD2_REF( U ) y)
{
    BOOST_STATIC_ASSERT((!boost_ext::is_cv_or<T>::value));
    BOOST_STATIC_ASSERT((boost_ext::mpl::or2<
        boost::mpl::not_< boost_ext::is_reference<T> >,
        sake::is_convertible_wnrbt< SAKE_FWD2_PARAM( U ), T >
    >::value));
    return static_cast<T>(sake::forward<U>(y));
}

namespace construct
{

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class... U >
inline sake::emplacer< void ( U... ) >
emplacer(U&&... y)
{ return sake::emplacer< void ( U... ) >(sake::forward<U>(y)...); }

template< class T, class... U >
inline sake::emplacer< T ( U... ) >
emplacer(U&&... y)
{ return sake::emplacer< T ( U... ) >(sake::forward<U>(y)...); }

#else // #if !defined( ... ) && ...

inline sake::emplacer< void ( ) >
emplacer()
{ return sake::emplacer< void ( ) >(); }

#define SAKE_OVERLOAD_RESULT( r, n, U_tuple ) \
    boost::mpl::identity< sake::emplacer< void U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    emplacer
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, y_tuple, forward_y_tuple ) \
    return sake::emplacer< void U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

template< class T >
inline sake::emplacer< T ( ) >
emplacer()
{ return sake::emplacer< T ( ) >(); }

#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, n, classU_tuple ) \
    class T, BOOST_PP_TUPLE_REM_CTOR( n, classU_tuple )
#define SAKE_OVERLOAD_RESULT( r, n, U_tuple ) \
    boost::mpl::identity< sake::emplacer< T U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    emplacer
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, y_tuple, forward_y_tuple ) \
    return sake::emplacer< T U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && ...

} // namespace construct

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_CONSTRUCT_HPP
