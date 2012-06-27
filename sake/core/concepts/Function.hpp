/*******************************************************************************
 * sake/core/concepts/Function.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::Function< F, R, T... >
 * struct concepts::Function< F, R ( T... ) >
 * struct concepts::Function< F ( T... ) >
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_CONCEPTS_FUNCTION_HPP
#define SAKE_CORE_CONCEPTS_FUNCTION_HPP

#include <boost/concept/usage.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

#include <sake/core/utility/declval.hpp>

#ifndef SAKE_CONCEPTS_FUNCTION_MAX_ARITY
#define SAKE_CONCEPTS_FUNCTION_MAX_ARITY 8
#endif // #ifndef SAKE_CONCEPTS_FUNCTION_MAX_ARITY

namespace sake
{

namespace concepts
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class F, class... T >
struct Function;

template< class F, class R, class... T >
struct Function< F, R, T... >
{
    BOOST_CONCEPT_USAGE( Function )
    {
        assert_result(f(sake::declval<T>()...));
    }

private:
    F f;

    void assert_result(R);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Function )
};

template< class F, class... T >
struct Function< F, void, T... >
{
    BOOST_CONCEPT_USAGE( Function )
    {
        f(sake::declval<T>()...);
    }

private:
    F f;

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Function )
};

template< class F, class R, class... T >
struct Function< F, R ( T... ) >
    : sake::concepts::Function< F, R, T... >
{ };

template< class F, class... T >
struct Function< F ( T... ) >
    : sake::concepts::Function< F, void, T... >
{ };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

template<
    class F,
    class R = void,
    BOOST_PP_ENUM_BINARY_PARAMS(
        SAKE_CONCEPTS_FUNCTION_MAX_ARITY,
        class T, = void BOOST_PP_INTERCEPT
    )
>
struct Function;

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_CONCEPTS_FUNCTION_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/concepts/Function.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_FUNCTION_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_T0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class T )
#define comma_T0N       BOOST_PP_ENUM_TRAILING_PARAMS( N, T )
#define declval_T0N     BOOST_PP_ENUM_BINARY_PARAMS( N, sake::declval< T, >() BOOST_PP_INTERCEPT )
#define T0N             BOOST_PP_ENUM_PARAMS( N, T )

template< class F, class R comma_class_T0N >
#if N == SAKE_CONCEPTS_FUNCTION_MAX_ARITY
struct Function
#else // #if N == SAKE_CONCEPTS_FUNCTION_MAX_ARITY
struct Function< F, R comma_T0N >
#endif // #if N == SAKE_CONCEPTS_FUNCTION_MAX_ARITY
{
    BOOST_CONCEPT_USAGE( Function )
    {
        assert_result(f(declval_T0N));
    }

private:
    F f;

    void assert_result(R);

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Function )
};

template< class F comma_class_T0N >
struct Function< F, void comma_T0N >
{
    BOOST_CONCEPT_USAGE( Function )
    {
        f(declval_T0N);
    }

private:
    F f;

    SAKE_SUPPRESS_WARNING_UNINIITIALIZED_IN_CONCEPT_CHECKING_CLASS( Function )
};

template< class F, class R comma_class_T0N >
struct Function< F, R ( T0N ) >
    : sake::concepts::Function< F, R comma_T0N >
{ };

template< class F comma_class_T0N >
struct Function< F ( T0N ) >
    : sake::concepts::Function< F, void comma_T0N >
{ };

#undef comma_class_T0N
#undef comma_T0N
#undef declval_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
