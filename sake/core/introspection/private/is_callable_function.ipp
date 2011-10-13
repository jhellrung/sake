/*******************************************************************************
 * sake/core/introspection/private/is_callable_function.ipp
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
    Result SAKE_INTROSPECTION_FUNCTION_NAME T_tuple ;
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_DECLARE

#ifndef SAKE_INTROSPECTION_FUNCTION_APPLY
#define SAKE_INTROSPECTION_FUNCTION_APPLY( x_tuple ) \
    SAKE_INTROSPECTION_FUNCTION_NAME x_tuple
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_APPLY

#ifndef SAKE_INTROSPECTION_FUNCTION_ARITY
#ifndef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#define min_arity 1
#define max_arity SAKE_INTROSPECTION_DEFAULT_MAX_ARITY
#else // #ifndef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#define min_arity BOOST_PP_TUPLE_ELEM( 2, 0, SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS )
#define max_arity BOOST_PP_TUPLE_ELEM( 2, 1, SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS )
#if !(1 <= min_arity && min_arity <= max_arity)
#error Invalid SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS.
#endif // #if !(1 <= min_arity && min_arity <= max_arity)
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#else // #ifndef SAKE_INTROSPECTION_FUNCTION_ARITY
#ifdef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#error Must not define both SAKE_INTROSPECTION_FUNCTION_ARITY and\
 SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#endif // #ifdef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
#if SAKE_INTROSPECTION_FUNCTION_ARITY < 1
#error SAKE_INTROSPECTION_FUNCTION_ARITY must be at least 1.
#endif // #if SAKE_INTROSPECTION_FUNCTION_ARITY < 1
#define min_arity SAKE_INTROSPECTION_FUNCTION_ARITY
#define max_arity SAKE_INTROSPECTION_FUNCTION_ARITY
#endif // #ifndef SAKE_INTROSPECTION_FUNCTION_ARITY



template<
    class Signature,
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;

namespace extension
{
template< class Signature, class ResultPred, class Enable = void >
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace extension

namespace default_impl
{
template<
    class Signature,
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace default_impl



/*******************************************************************************
 * struct SAKE_INTROSPECTION_TRAIT_NAME< Signature, ResultPred = ... >
 ******************************************************************************/

template< class Signature, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : extension::SAKE_INTROSPECTION_TRAIT_NAME< Signature, ResultPred >
{ };

/*******************************************************************************
 * struct extension::SAKE_INTROSPECTION_TRAIT_NAME< Signature, ResultPred, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class Signature, class ResultPred, class Enable /*= void*/ >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : default_impl::SAKE_INTROSPECTION_TRAIT_NAME< Signature, ResultPred >
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::SAKE_INTROSPECTION_TRAIT_NAME< Signature, ResultPred = ... >
 ******************************************************************************/

namespace default_impl
{

#define trait_name_private BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )

namespace trait_name_private
{

template< class Signature >
struct has_void_result;
template< class Signature, class ResultPred >
struct non_void_result_helper;

#ifdef SAKE_INTROSPECTION_FUNCTION_ARITY
SAKE_INTROSPECTION_FUNCTION_DECLARE(
    ::sake::introspection_private::dummy,
    ( BOOST_PP_ENUM_PARAMS(
        SAKE_INTROSPECTION_FUNCTION_ARITY,
        ::sake::convertible_from_any BOOST_PP_INTERCEPT
    ) )
)
#else // #ifdef SAKE_INTROSPECTION_FUNCTION_ARITY
SAKE_INTROSPECTION_FUNCTION_DECLARE(
    ::sake::introspection_private::dummy,
    ( ... )
)
#endif // #ifdef SAKE_INTROSPECTION_FUNCTION_ARITY

} // namespace trait_name_private

#if !defined( SAKE_INTROSPECTION_FUNCTION_ARITY ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class Result, class... T, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME< Result ( T... ), ResultPred >
    : ::sake::boost_ext::mpl::and2<
          ::boost::mpl::not_< trait_name_private::has_void_result< void ( T... ) > >,
          trait_name_private::non_void_result_helper< Result ( T... ), ResultPred >
      >
{ };

template< class... T, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME< void ( T... ), ResultPred >
{
    static bool const value = ::boost::mpl::eval_if_c<
        trait_name_private::has_void_result< void ( T... ) >::value,
        ::boost::mpl::apply1< ResultPred, void >,
        trait_name_private::non_void_result_helper< void ( T... ), ResultPred >
    >::type::value;
    typedef SAKE_INTROSPECTION_TRAIT_NAME type;
};

namespace trait_name_private
{

template< class... T >
struct has_void_result< void ( T... ) >
{
    static bool const value = SAKE_EXPR_IS_VOID(
        SAKE_INTROSPECTION_FUNCTION_APPLY( (::sake::declval<T>()...) ) );
    typedef has_void_result type;
};

template< class Result, class... T, class ResultPred >
struct non_void_result_helper< Result ( T... ), ResultPred >
{
    static bool const value =
       !SAKE_EXPR_IS_CONVERTIBLE(
            SAKE_INTROSPECTION_FUNCTION_APPLY( (::sake::declval<T>()...) ),
            ::sake::introspection_private::dummy
        )
     && SAKE_EXPR_IS_CONVERTIBLE(
            SAKE_INTROSPECTION_FUNCTION_APPLY( (::sake::declval<T>()...) ),
            Result
        )
     && SAKE_EXPR_APPLY(
            ResultPred,
            SAKE_INTROSPECTION_FUNCTION_APPLY( (::sake::declval<T>()...) )
        );
    typedef non_void_result_helper type;
};

} // namespace trait_name_private

#else // #if !defined( ... ) && !defined( ... )

#define BOOST_PP_ITERATION_LIMITS ( min_arity, max_arity )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/private/is_callable_function.ipp>
#include BOOST_PP_ITERATE()

#endif // #if !defined( ... ) && !defined( ... )

#undef trait_name_private

} // namespace default_impl



#undef min_arity
#undef max_arity



#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_FUNCTION_NAME
#undef SAKE_INTROSPECTION_FUNCTION_DECLARE
#undef SAKE_INTROSPECTION_FUNCTION_APPLY
#undef SAKE_INTROSPECTION_FUNCTION_ARITY
#undef SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N       BOOST_PP_ENUM_PARAMS( N, T )
#define apply_declval_T0N \
    SAKE_INTROSPECTION_FUNCTION_APPLY( \
        ( BOOST_PP_ENUM_BINARY_PARAMS( N, ::sake::declval< T, >() BOOST_PP_INTERCEPT ) ) )

template< class Result, class_T0N, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME< Result ( T0N ), ResultPred >
    : ::sake::boost_ext::mpl::and2<
          ::boost::mpl::not_< trait_name_private::has_void_result< void ( T0N ) > >,
          trait_name_private::non_void_result_helper< Result ( T0N ), ResultPred >
      >
{ };

template< class_T0N, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME< void ( T0N ), ResultPred >
{
    static bool const value = ::boost::mpl::eval_if_c<
        trait_name_private::has_void_result< void ( T0N ) >::value,
        ::boost::mpl::apply1< ResultPred, void >,
        trait_name_private::non_void_result_helper< void ( T0N ), ResultPred >
    >::type::value;
    typedef SAKE_INTROSPECTION_TRAIT_NAME type;
};

namespace trait_name_private
{

template< class_T0N >
struct has_void_result< void ( T0N ) >
{
    static bool const value = SAKE_EXPR_IS_VOID( apply_declval_T0N );
    typedef has_void_result type;
};

template< class Result, class_T0N, class ResultPred >
struct non_void_result_helper< Result ( T0N ), ResultPred >
{
    static bool const value =
       !SAKE_EXPR_IS_CONVERTIBLE( apply_declval_T0N, ::sake::introspection_private::dummy )
     && SAKE_EXPR_IS_CONVERTIBLE( apply_declval_T0N, Result )
     && SAKE_EXPR_APPLY( ResultPred, apply_declval_T0N );
    typedef non_void_result_helper type;
};

} // namespace trait_name_private

#undef class_T0N
#undef T0N
#undef apply_declval_T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
