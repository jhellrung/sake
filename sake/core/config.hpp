/*******************************************************************************
 * sake/core/config.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_MSC_VERSION
 * #define SAKE_GNUC_VERSION
 *
 * #define SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FUN_DEF
 * #define SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
 *
 * This provides macros to facilitate workarounds for MSVC and GCC compilers.
 *
 * SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FUN_DEF
 * --------
 * MSVC does not require (and, indeed, errors if included) the "template"
 * keyword in the return type of member function definitions when the standard
 * otherwise requires it.  To maintain portability, use this macro in place of
 * this "template" keyword.  For example:
 *
 * template< class T >
 * struct X
 * {
 *     template< class U >
 *     struct Y { typedef void type; };
 *     template< class U >
 *     Y<U>::type f(U);
 * };
 *
 * template< class T >
 * template< class U >
 * typename X<T>:: SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FUN_DEF Y<U>::type
 * X<T>::
 * f(U)
 * { [...] }
 *
 * SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
 * --------
 * Older versions of GCC find non-functions (in addition to functions) during
 * ADL, which is almost always (actually, probably always) undesirable.  The
 * workaround is to bury the non-function in an adl barrier namespace and use a
 * using directive to bring the identifier into the outer scope.  For example:
 *
 * #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
 * namespace X_adl_barrier
 * { X const x = { }; }
 * using namespace X_adl_barrier;
 * #else // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
 * X const x = { };
 * #endif // #ifdef SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
 ******************************************************************************/

#ifndef SAKE_CORE_CONFIG_HPP
#define SAKE_CORE_CONFIG_HPP

#ifdef _MSC_VER
#define SAKE_MSC_VERSION \
    _MSC_VER
#endif // #ifdef _MSC_VER

#define SAKE_GNUC_VERSION_OF( major, minor, patch ) \
    ( 10000 * major + 100 * minor + patch )

#ifdef __GNUC__
#define SAKE_GNUC_VERSION \
    SAKE_GNUC_VERSION_OF( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__ )
#endif // #ifdef __GNUC__

#if SAKE_MSC_VERSION <= 1500
#define SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FUN_DEF
#else // #if SAKE_MSC_VERSION <= 1500
#define SAKE_WORKAROUND_NESTED_TEMPLATE_IN_MEM_FUN_DEF template
#endif // #if SAKE_MSC_VERSION <= 1500

#if SAKE_GNUC_VERSION <= SAKE_GNUC_VERSION_OF(4,4,9)
#define SAKE_WORKAROUND_ADL_FINDS_NON_FUNCTIONS
#endif // #if SAKE_GNUC_VERSION <= SAKE_GNUC_VERSION_OF(4,4,9)

#endif // #ifndef SAKE_CORE_CONFIG_HPP