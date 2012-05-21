/*******************************************************************************
 * sake/core/emplacer/make.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * make_emplacer(U0&& y0, ...) -> emplacer< void ( U0, ... ) >
 * make_emplacer<T>(U0&& y0, ...) -> emplacer< T ( U0, ... ) >
 ******************************************************************************/

#ifndef SAKE_CORE_EMPLACER_MAKE_HPP
#define SAKE_CORE_EMPLACER_MAKE_HPP

#include <boost/config.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/tuple/rem.hpp>

#include <sake/core/emplacer/emplacer.hpp>
#include <sake/core/emplacer/fwd.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/overload.hpp>

namespace sake
{

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class... U >
inline sake::emplacer< void ( U... ) >
make_emplacer(U&&... y)
{ return sake::emplacer< void ( U... ) >(sake::forward<U>(y)...); }

template< class T, class... U >
inline sake::emplacer< T ( U... ) >
make_emplacer(U&&... y)
{ return sake::emplacer< T ( U... ) >(sake::forward<U>(y)...); }

#else // #if !defined( ... ) && ...

inline sake::emplacer< void ( ) >
make_emplacer()
{ return sake::emplacer< void ( ) >(); }

#define SAKE_OVERLOAD_RESULT( r, n, U_tuple ) \
    boost::mpl::identity< sake::emplacer< void U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_emplacer
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, y_tuple, forward_y_tuple ) \
    return sake::emplacer< void U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

template< class T >
inline sake::emplacer< T ( ) >
make_emplacer()
{ return sake::emplacer< T ( ) >(); }

#define SAKE_OVERLOAD_DECLARE_TEMPLATE_PARAMS( r, n, classU_tuple ) \
    class T, BOOST_PP_TUPLE_REM_CTOR( n, classU_tuple )
#define SAKE_OVERLOAD_RESULT( r, n, U_tuple ) \
    boost::mpl::identity< sake::emplacer< T U_tuple > >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_emplacer
#define SAKE_OVERLOAD_BODY( r, n, U_tuple, y_tuple, forward_y_tuple ) \
    return sake::emplacer< T U_tuple > forward_y_tuple ;
#define SAKE_OVERLOAD_PERFECT_MAX_ARITY SAKE_EMPLACER_PERFECT_MAX_ARITY
#define SAKE_OVERLOAD_FWD_MAX_ARITY     SAKE_EMPLACER_FWD_MAX_ARITY
#define SAKE_OVERLOAD_FWD2_MAX_ARITY    SAKE_EMPLACER_FWD2_MAX_ARITY
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && ...

} // namespace sake

#endif // #ifndef SAKE_CORE_EMPLACER_MAKE_HPP
