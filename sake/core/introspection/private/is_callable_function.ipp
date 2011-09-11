/*******************************************************************************
 * core/introspection/private/is_callable_function.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_INTROSPECTION_TRAIT_NAME
#error SAKE_INTROSPECTION_TRAIT_NAME not defined.
#endif // #ifndef SAKE_INTROSPECTION_TRAIT_NAME

#if !defined( SAKE_INTROSPECTION_FUNCTION_NAME ) \
&& (!defined( SAKE_INTROSPECTION_FUNCTION_DECLARE ) \
 && !defined( SAKE_INTROSPECTION_FUNCTION_APPLY ))
#error SAKE_INTROSPECTION_FUNCTION_NAME not defined.
#endif // #if !defined( ... ) && ...

#ifndef SAKE_INTROSPECTION_FUNCTION_DECLARE
#define SAKE_INTROSPECTION_FUNCTION_DECLARE( Result, T_tuple ) \
    Result SAKE_INTROSPECTION_FUNCTION_NAME ( T_tuple );
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_DECLARE

#ifndef SAKE_INTROSPECTION_FUNCTION_APPLY
#define SAKE_INTROSPECTION_FUNCTION_APPLY( x_tuple ) \
    SAKE_INTROSPECTION_FUNCTION_NAME x_tuple
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_APPLY

#ifndef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#define SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS \
    ( 1, SAKE_INTROSPECTION_FUNCTION_DEFAULT_MAX_ARITY )
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS



#define min_arity BOOST_PP_TUPLE_ELEM( 2, 0, SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS )
#define max_arity BOOST_PP_TUPLE_ELEM( 2, 1, SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS )
#if !(1 <= min_arity && min_arity <= max_arity)
#error Invalid SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS.
#endif // #if !(...)

#define trait_name_private BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )



template<
    class Signature,
    class ResultMetafunction = ::boost::mpl::always< ::boost::true_type >
> struct SAKE_INTROSPECTION_TRAIT_NAME;



namespace no_ext
{

template< class Signature, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME;

namespace trait_name_private
{

template< class Signature >
struct has_void_result;
template< class Signature, class ResultMetafunction >
struct non_void_result_helper;

#if min_arity == max_arity
SAKE_INTROSPECTION_FUNCTION_DECLARE(
    ::sake::introspection_private::dummy,
    ( BOOST_PP_ENUM_PARAMS(
        min_arity,
        ::sake::convertible_from_any BOOST_PP_INTERCEPT
    ) )
)
#else // #if min_arity == max_arity
SAKE_INTROSPECTION_FUNCTION_DECLARE(
    ::sake::introspection_private::dummy,
    ( ... )
)
#endif // #if min_arity == max_arity

} // namespace trait_name_private

#define BOOST_PP_ITERATION_LIMITS SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#define BOOST_PP_FILENAME_1       <sake/core/introspection/private/is_callable_function.ipp>
#include BOOST_PP_ITERATE()

} // namespace no_ext



SAKE_EXTENSION_CLASS( SAKE_INTROSPECTION_TRAIT_NAME, 2 )

template< class Signature, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : ext::SAKE_INTROSPECTION_TRAIT_NAME< Signature, ResultMetafunction >
{ };



#undef min_arity
#undef max_arity
#undef trait_name_private



#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_FUNCTION_NAME
#undef SAKE_INTROSPECTION_FUNCTION_DECLARE
#undef SAKE_INTROSPECTION_FUNCTION_APPLY
#undef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )
#define apply_declval_T0N \
    SAKE_INTROSPECTION_FUNCTION_APPLY( ( \
        BOOST_PP_ENUM_BINARY_PARAMS( N, ::sake::declval< T, >() BOOST_PP_INTERCEPT ) \
    ) )

template< class Result, class_T0N, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME< Result ( T0N ), ResultMetafunction >
    : ::boost::mpl::and_<
          ::boost::mpl::not_< trait_name_private::has_void_result< void ( T0N ) > >,
          trait_name_private::non_void_result_helper< Result ( T0N ), ResultMetafunction >
      >
{ };

template< class_T0N, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME< void ( T0N ), ResultMetafunction >
    : ::boost::mpl::eval_if_c<
          trait_name_private::has_void_result< void ( T0N ) >::value,
          ::boost::mpl::apply1< ResultMetafunction, void >,
          trait_name_private::non_void_result_helper< void ( T0N ), ResultMetafunction >
      >
{ };

namespace trait_name_private
{

template< class_T0N >
struct has_void_result< void ( T0N ) >
{
    static const bool value = SAKE_EXPR_IS_VOID( apply_declval_T0N );
    typedef has_void_result type;
};

template< class Result, class_T0N, class ResultMetafunction >
struct non_void_result_helper< Result ( T0N ), ResultMetafunction >
{
    static const bool value =
       !SAKE_EXPR_IS_CONVERTIBLE( apply_declval_T0N, ::sake::introspection_private::dummy )
     && SAKE_EXPR_IS_CONVERTIBLE( apply_declval_T0N, Result )
     && SAKE_EXPR_APPLY( ResultMetafunction, apply_declval_T0N );
    typedef non_void_result_helper type;
};

} // namespace trait_name_private

#undef class_T0N
#undef T0N
#undef apply_declval_T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
