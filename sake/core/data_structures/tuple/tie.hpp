/*******************************************************************************
 * sake/core/data_structures/tuple/tie.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * tie(T0& x0, ... ) -> result_of::tie< T0&, ... >::type
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_TIE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_TIE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/utility/ignore.hpp>

namespace sake
{

/*******************************************************************************
 * struct result_of::tie< T0, ... >
 ******************************************************************************/

namespace result_of
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_BINARY_PARAMS( SAKE_TUPLE_MAX_SIZE, class T, = void BOOST_PP_INTERCEPT ) >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct tie;

} // namespace result_of

/*******************************************************************************
 * tie(T0& x0, ... ) -> result_of::tie< T0&, ... >::type
 ******************************************************************************/

inline sake::tuple<>
tie()
{ return sake::tuple<>(); }

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class... T >
inline typename sake::result_of::tie< T... >::type
tie(T&... x)
{ return typename sake::result_of::tie< T&... >::type(x...); }

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

namespace tie_private
{

struct assignable_from_any
{
    assignable_from_any(sake::ignore_t)
    { }

    template< class T >
    void operator=(T const &)
    { }
};

template< class T >
struct replace_ignore
{ typedef T type; };

template<>
struct replace_ignore< sake::ignore_t >
{ typedef assignable_from_any type; };

template<>
struct replace_ignore< sake::ignore_t& >
{ typedef assignable_from_any type; };

template<>
struct replace_ignore< sake::ignore_t const & >
{ typedef assignable_from_any type; };

} // namespace tie_private

#define replace_ignore_Tn( z, n, data ) \
    typename sake::tie_private::replace_ignore< T ## n & >::type

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/tuple/tie.hpp>
#include BOOST_PP_ITERATE()

#undef replace_ignore_Tn

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_TIE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

namespace result_of
{

template< class_T0N >
#if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct tie
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct tie< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
{ typedef sake::tuple< BOOST_PP_ENUM( N, replace_ignore_Tn, ~ ) > type; };

} // namespace result_of

#ifdef BOOST_NO_VARIADIC_TEMPLATES

#define T0N_ref     BOOST_PP_ENUM_BINARY_PARAMS( N, T, & BOOST_PP_INTERCEPT )
#define T0N_ref_x0N BOOST_PP_ENUM_BINARY_PARAMS( N, T, & x )
#define x0N         BOOST_PP_ENUM_PARAMS( N, x )

template< class_T0N >
inline typename sake::result_of::tie< T0N_ref >::type
tie(T0N_ref_x0N)
{
    typedef typename sake::result_of::tie< T0N_ref >::type result_type;
    return result_type(x0N);
}

#undef T0N_ref
#undef T0N_ref_x0N
#undef x0N

#endif // #ifdef BOOST_NO_VARIADIC_TEMPLATES

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
