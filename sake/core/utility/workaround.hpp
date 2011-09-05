/*******************************************************************************
 * core/utility/workaround.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FN_DEF
 *
 * SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version )
 * SAKE_WORKAROUND_EXPR_IF_MSVC_VERSION_LESS_EQUAL( version, Expr )
 * SAKE_WORKAROUND_EXPR_UNLESS_MSVC_VERSION_LESS_EQUAL( version, Expr )
 * SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL( version, Expr )
 * SAKE_WORKAROUND_COMMA_EXPR_UNLESS_MSVC_VERSION_LESS_EQUAL( version, Expr )
 *
 * SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple )
 * SAKE_WORKAROUND_EXPR_IF_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr )
 * SAKE_WORKAROUND_EXPR_UNLESS_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr )
 * SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr )
 * SAKE_WORKAROUND_COMMA_EXPR_UNLESS_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr )
 *
 * This provides a number of macros to facilitate workarounds for MSVC and GCC
 * compilers.
 *
 * Of particular note is SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FN_DEF.  MSVC
 * does not require (and, indeed, errors if included) the "template" keyword in
 * the return type of member function definitions when the standard otherwise
 * requires it.  To maintain portability, use the aforementioned macro in place
 * of this "template" keyword.  For example:
 *
 * template< class T >
 * struct X
 * {
 *     template< class U >
 *     struct Y { typedef void type; };
 *     template< class U >
 *     Y<T>::type f(U);
 * };
 *
 * template< class T >
 * template< class U >
 * typename X<T>:: SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FN_DEF Y<U>::type
 * f(U)
 * { }
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_WORKAROUND_HPP
#define SAKE_CORE_UTILITY_WORKAROUND_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/comparison/less_equal.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/logical/bitand.hpp>
#include <boost/preprocessor/logical/bitor.hpp>
#include <boost/preprocessor/logical/compl.hpp>

#ifdef _MSC_VER

#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version ) \
    BOOST_PP_CAT( SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_, version )

#if   _MSC_VER == 1300 // MSVC  7.0 / VS 2003
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1300 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1310 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1400 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1500 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1600 1
#elif _MSC_VER == 1310 // MSVC  7.1 / VS 2003 1.0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1300 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1310 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1400 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1500 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1600 1
#elif _MSC_VER == 1400 // MSVC  8.0 / VS 2005
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1300 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1310 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1400 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1500 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1600 1
#elif _MSC_VER == 1500 // MSVC  9.0 / VS 2008
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1300 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1310 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1400 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1500 1
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1600 1
#elif _MSC_VER == 1600 // MSVC 10.0 / VS 2010
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1300 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1310 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1400 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1500 0
#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL_1600 1
#else
#error Unrecognized value of _MSC_VER.
#endif

#define SAKE_WORKAROUND_EXPR_IF_MSVC_VERSION_LESS_EQUAL( version, Expr ) \
    BOOST_PP_EXPR_IIF( SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version ), Expr )
#define SAKE_WORKAROUND_EXPR_UNLESS_MSVC_VERSION_LESS_EQUAL( version, Expr ) \
    BOOST_PP_EXPR_IIF( BOOST_PP_COMPL( SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version ) ), Expr )

#define SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL( version, Expr ) \
    BOOST_PP_CAT( \
        SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL_, \
        SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version ) \
    ) ( Expr )
#define SAKE_WORKAROUND_COMMA_EXPR_UNLESS_MSVC_VERSION_LESS_EQUAL( version, Expr ) \
    BOOST_PP_CAT( \
        SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL_, \
        BOOST_PP_COMPL( SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version ) ) \
    ) ( Expr )

#define SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL_0( Expr )
#define SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL_1( Expr ) , Expr

#else // #ifdef _MSC_VER

#define SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( version ) 0
#define SAKE_WORKAROUND_EXPR_IF_MSVC_VERSION_LESS_EQUAL( version, Expr )
#define SAKE_WORKAROUND_EXPR_UNLESS_MSVC_VERSION_LESS_EQUAL( version, Expr ) Expr
#define SAKE_WORKAROUND_COMMA_EXPR_IF_MSVC_VERSION_LESS_EQUAL( version, Expr )
#define SAKE_WORKAROUND_COMMA_EXPR_UNLESS_MSVC_VERSION_LESS_EQUAL( version, Expr ) , Expr

#endif // #ifdef _MSC_VER

#ifdef __GNUC__

#define SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple ) \
    SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL_impl version_tuple

#define SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL_impl( major, minor, patch ) \
    BOOST_PP_BITOR( \
        BOOST_PP_LESS( __GNUC__, major ), \
        BOOST_PP_BITAND( \
            BOOST_PP_EQUAL( __GNUC__, major ), \
            BOOST_PP_BITOR( \
                BOOST_PP_LESS( __GNUC_MINOR__, minor ), \
                BOOST_PP_BITAND( \
                    BOOST_PP_EQUAL( __GNUC_MINOR__, minor ), \
                    BOOST_PP_LESS_EQUAL( __GNUC_PATCHLEVEL__, patch ) \
                ) \
            ) \
        ) \
    )

#define SAKE_WORKAROUND_EXPR_IF_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr ) \
    BOOST_PP_EXPR_IIF( SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple ), Expr )
#define SAKE_WORKAROUND_EXPR_UNLESS_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr ) \
    BOOST_PP_EXPR_IIF( BOOST_PP_COMPL( SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple ) ), Expr )

#define SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr ) \
    BOOST_PP_CAT( \
        SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL_, \
        SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple ) \
    ) ( Expr )
#define SAKE_WORKAROUND_COMMA_EXPR_UNLESS_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr ) \
    BOOST_PP_CAT( \
        SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL_, \
        BOOST_PP_COMPL( SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple ) ) \
    ) ( Expr )

#define SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL_0( Expr )
#define SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL_1( Expr ) , Expr

#else // #ifdef __GNUC__

#define SAKE_WORKAROUND_GNUC_VERSION_LESS_EQUAL( version_tuple ) 0
#define SAKE_WORKAROUND_EXPR_IF_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr )
#define SAKE_WORKAROUND_EXPR_UNLESS_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr ) Expr
#define SAKE_WORKAROUND_COMMA_EXPR_IF_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr )
#define SAKE_WORKAROUND_COMMA_EXPR_UNLESS_GNUC_VERSION_LESS_EQUAL( version_tuple, Expr ) , Expr

#endif // #ifdef __GNUC__

#if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1600 )
#define SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FN_DEF
#else // #if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1600 )
#define SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FN_DEF template
#endif // #if SAKE_WORKAROUND_MSVC_VERSION_LESS_EQUAL( 1600 )

#endif // #ifndef SAKE_CORE_UTILITY_WORKAROUND_HPP
