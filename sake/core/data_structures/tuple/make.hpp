/*******************************************************************************
 * sake/core/data_structures/tuple/make.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * make_tuple(T0&& x0, ... ) -> result_of::make_tuple< T0, ... >::type
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_MAKE_HPP
#define SAKE_CORE_DATA_STRUCTURES_TUPLE_MAKE_HPP

#include <boost/config.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/data_structures/tuple/tuple.hpp>
#include <sake/core/move/forward.hpp>
#include <sake/core/utility/overload.hpp>
#include <sake/core/utility/ref/wrapped_parameter_to_reference.hpp>

namespace sake
{

/*******************************************************************************
 * struct result_of::make_tuple< T0, ... >
 ******************************************************************************/

namespace result_of
{

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template< class... T >
#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES
template< BOOST_PP_ENUM_BINARY_PARAMS( SAKE_TUPLE_MAX_SIZE, class T, = void BOOST_PP_INTERCEPT ) >
#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES
struct make_tuple;

#define wrapped_parameter_to_reference_remove_qualifiers_Tn( z, n, data ) \
    typename sake::wrapped_parameter_to_reference< \
        typename boost_ext::remove_qualifiers< T ## n >::type >::type

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_TUPLE_MAX_SIZE )
#define BOOST_PP_FILENAME_1       <sake/core/data_structures/tuple/make.hpp>
#include BOOST_PP_ITERATE()

#undef wrapped_parameter_to_reference_remove_qualifiers_Tn

} // namespace result_of

/*******************************************************************************
 * make_tuple(T0&& x0, ... ) -> result_of::make_tuple< T0, ... >::type
 ******************************************************************************/

inline sake::tuple<>
make_tuple()
{ return sake::tuple<>(); }

#if !defined( BOOST_NO_RVALUE_REFERENCES ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class... T >
inline typename sake::result_of::make_tuple< T... >::type
make_tuple(T&&... x)
{ return typename sake::result_of::make_tuple< T... >::type(sake::forward<T>(x)...); }

#else // #if !defined( ... ) && ...

#define SAKE_OVERLOAD_RESULT( r, n, T_tuple ) \
    sake::result_of::make_tuple< BOOST_PP_TUPLE_REM_CTOR( n, T_tuple ) >
#define SAKE_OVERLOAD_FUNCTION_NAME \
    make_tuple
#define SAKE_OVERLOAD_BODY( r, n, T_tuple, x_tuple, forward_x_tuple ) \
    return typename SAKE_OVERLOAD_RESULT( r, n, T_tuple ) ::type forward_x_tuple ;
#define SAKE_OVERLOAD_FWD_MAX_ARITY SAKE_TUPLE_MAX_SIZE
#include SAKE_OVERLOAD_GENERATE()

#endif // #if !defined( ... ) && ...

} // namespace sake

#endif // #ifndef SAKE_CORE_DATA_STRUCTURES_TUPLE_MAKE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )

template< class_T0N >
#if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct make_tuple
#else // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
struct make_tuple< T0N >
#endif // #if defined( BOOST_NO_VARIADIC_TEMPLATES ) && N == SAKE_TUPLE_MAX_SIZE
{
    typedef sake::tuple< BOOST_PP_ENUM( N,
        wrapped_parameter_to_reference_remove_qualifiers_Tn, ~ ) > type;
};

#undef class_T0N
#undef T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
