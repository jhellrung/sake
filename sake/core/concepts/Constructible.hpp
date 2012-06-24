/*******************************************************************************
 * sake/core/concepts/Constructible.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct concepts::Constructible< T, U... >
 * struct concepts::Constructible< T ( U... ) >
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_CONCEPTS_CONSTRUCTIBLE_HPP
#define SAKE_CORE_CONCEPTS_CONSTRUCTIBLE_HPP

#include <boost/concept/usage.hpp>
#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>

#include <sake/core/utility/declval.hpp>

#ifndef SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY
#define SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY 8
#endif // #ifndef SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY

namespace sake
{

namespace concepts
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class T, class... U >
struct Constructible
{
    BOOST_CONCEPT_USAGE( Constructible )
    {
        T x(sake::declval<U>()...);
        static_cast<void>(x);
    }
};

template< class T, class... U >
struct Constructible< T ( U... ) >
    : sake::concepts::Constructible< T, U... >
{ };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

template<
    class T,
    BOOST_PP_ENUM_BINARY_PARAMS(
        SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY,
        class U, = void BOOST_PP_INTERCEPT )
>
struct Constructible;

#define BOOST_PP_ITERATION_LIMITS ( 1, SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/concepts/Constructible.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class T >
struct Constructible<T>
{
    BOOST_CONCEPT_USAGE( Constructible )
    {
        T x;
        static_cast<void>(x);
    }
};

template< class T >
struct Constructible< T ( ) >
    : sake::concepts::Constructible<T>
{ };

} // namespace concepts

} // namespace sake

#endif // #ifndef SAKE_CORE_CONCEPTS_CONSTRUCTIBLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define comma_U0N       BOOST_PP_ENUM_TRAILING_PARAMS( N, U )
#define declval_U0N     BOOST_PP_ENUM_BINARY_PARAMS( N, sake::declval< U, >() BOOST_PP_INTERCEPT )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

template< class T comma_class_U0N >
#if N == SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY
struct Constructible
#else // #if N == SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY
struct Constructible< T comma_U0N >
#endif // #if N == SAKE_CONCEPTS_CONSTRUCTIBLE_MAX_ARITY
{
    BOOST_CONCEPT_USAGE( Constructible )
    {
        T x(declval_U0N);
        static_cast<void>(x);
    }
};

template< class T comma_class_U0N >
struct Constructible< T ( U0N ) >
    : sake::concepts::Constructible< T comma_U0N >
{ };

#undef comma_class_U0N
#undef comma_U0N
#undef declval_U0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
