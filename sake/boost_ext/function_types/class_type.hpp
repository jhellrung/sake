/*******************************************************************************
 * sake/boost_ext/function_types/class_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::function_types::class_type<F>
 *
 * This is a metafunction which evaluates to the class type of a
 * pointer-to-member-function or pointer-to-member-object.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_FUNCTION_TYPES_CLASS_TYPE_HPP
#define SAKE_BOOST_EXT_FUNCTION_TYPES_CLASS_TYPE_HPP

#include <boost/config.hpp>
#include <boost/function_types/config/config.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

namespace sake
{

namespace boost_ext
{

namespace function_types
{

template< class F >
struct class_type;

template< class R, class T >
struct class_type< R T::* >
{ typedef T type; };

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class R, class T, class... U >
struct class_type< R (T::*)( U... ) >
{ typedef T type; };

template< class R, class T, class... U >
struct class_type< R (T::*)( U... ) const >
{ typedef T const type; };

template< class R, class T, class... U >
struct class_type< R (T::*)( U... ) volatile >
{ typedef T volatile type; };

template< class R, class T, class... U >
struct class_type< R (T::*)( U... ) const volatile >
{ typedef T const volatile type; };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_FT_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/boost_ext/function_types/class_type.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace function_types

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUNCTION_TYPES_CLASS_TYPE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

template< class R, class T comma_class_U0N >
struct class_type< R (T::*)( U0N ) >
{ typedef T type; };

template< class R, class T comma_class_U0N >
struct class_type< R (T::*)( U0N ) const >
{ typedef T const type; };

template< class R, class T comma_class_U0N >
struct class_type< R (T::*)( U0N ) volatile >
{ typedef T volatile type; };

template< class R, class T comma_class_U0N >
struct class_type< R (T::*)( U0N ) const volatile >
{ typedef T const volatile type; };

#undef comma_class_U0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
