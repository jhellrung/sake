/*******************************************************************************
 * sake/boost_ext/function_types/replace_result_type.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::function_types::replace_result_type<F,S>
 *
 * This replaces what would be returned by
 *     boost::function_types::result_type<F>
 * with S.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_BOOST_EXT_FUNCTION_TYPES_REPLACE_RESULT_TYPE_HPP
#define SAKE_BOOST_EXT_FUNCTION_TYPES_REPLACE_RESULT_TYPE_HPP

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

template< class F, class S >
struct replace_result_type;

template< class R, class T, class S >
struct replace_result_type< R T::*, S >
{ typedef S T::*type; };

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class R, class... U, class S >
struct replace_result_type< R ( U... ), S >
{ typedef S type ( U... ); };

template< class R, class... U, class S >
struct replace_result_type< R (*)( U... ), S >
{ typedef S (*type)( U... ); };

template< class R, class... U, class S >
struct replace_result_type< R (&)( U... ), S >
{ typedef S (&type)( U... ); };

template< class R, class T, class... U, class S >
struct replace_result_type< R (T::*)( U... ), S >
{ typedef S (T::*type)( U... ); };

template< class R, class T, class... U, class S >
struct replace_result_type< R (T::*)( U... ) const, S >
{ typedef S (T::*type)( U... ) const; };

template< class R, class T, class... U, class S >
struct replace_result_type< R (T::*)( U... ) volatile, S >
{ typedef S (T::*type)( U... ) volatile; };

template< class R, class T, class... U, class S >
struct replace_result_type< R (T::*)( U... ) const volatile, S >
{ typedef S (T::*type)( U... ) const volatile; };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 0, BOOST_FT_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/boost_ext/function_types/replace_result_type.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace function_types

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_FUNCTION_TYPES_REPLACE_RESULT_TYPE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

template< class R comma_class_U0N, class S >
struct replace_result_type< R ( U0N ), S >
{ typedef S type ( U0N ); };

template< class R comma_class_U0N, class S >
struct replace_result_type< R (*)( U0N ), S >
{ typedef S (*type)( U0N ); };

template< class R comma_class_U0N, class S >
struct replace_result_type< R (&)( U0N ), S >
{ typedef S (&type)( U0N ); };

template< class R, class T comma_class_U0N, class S >
struct replace_result_type< R (T::*)( U0N ), S >
{ typedef S (T::*type)( U0N ); };

template< class R, class T comma_class_U0N, class S >
struct replace_result_type< R (T::*)( U0N ) const, S >
{ typedef S (T::*type)( U0N ) const; };

template< class R, class T comma_class_U0N, class S >
struct replace_result_type< R (T::*)( U0N ) volatile, S >
{ typedef S (T::*type)( U0N ) volatile; };

template< class R, class T comma_class_U0N, class S >
struct replace_result_type< R (T::*)( U0N ) const volatile, S >
{ typedef S (T::*type)( U0N ) const volatile; };

#undef comma_class_U0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
