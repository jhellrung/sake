/*******************************************************************************
 * sake/core/introspection/private/has_operator_unary.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_INTROSPECTION_TRAIT_NAME
#error SAKE_INTROSPECTION_TRAIT_NAME not defined.
#endif // #ifndef SAKE_INTROSPECTION_TRAIT_NAME

#if !defined( SAKE_INTROSPECTION_OPERATOR_NAME ) \
&& (!defined( SAKE_INTROSPECTION_OPERATOR_DECLARE ) \
 || !defined( SAKE_INTROSPECTION_OPERATOR_APPLY ))
#error SAKE_INTROSPECTION_OPERATOR_NAME not defined.
#endif // #if !defined( ... ) && ...

#ifndef SAKE_INTROSPECTION_OPERATOR_DECLARE
#define SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, T ) \
    Result operator SAKE_INTROSPECTION_OPERATOR_NAME ( T );
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_DECLARE

#ifndef SAKE_INTROSPECTION_OPERATOR_APPLY
#define SAKE_INTROSPECTION_OPERATOR_APPLY( x ) \
    SAKE_INTROSPECTION_OPERATOR_NAME x
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_APPLY

#ifndef SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T ) void
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT

#ifndef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, Result, ResultPred ) ::boost::false_type
#endif // #ifndef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR



template<
    class T,
    class Result = SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;

namespace extension
{
template< class T, class Result, class ResultPred, class Enable = void >
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace extension

namespace default_impl
{
template<
    class T,
    class Result = SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace default_impl



/*******************************************************************************
 * struct SAKE_INTROSPECTION_TRAIT_NAME< T, Result = ..., ResultPred = ... >
 ******************************************************************************/

template< class T, class Result, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : extension::SAKE_INTROSPECTION_TRAIT_NAME< T, Result, ResultPred >
{ };

/*******************************************************************************
 * struct extension::SAKE_INTROSPECTION_TRAIT_NAME< T, Result, ResultPred, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Result, class ResultPred, class Enable /*= void*/ >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : default_impl::SAKE_INTROSPECTION_TRAIT_NAME< T, Result, ResultPred >
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::SAKE_INTROSPECTION_TRAIT_NAME< T, Result = ..., ResultPred = ... >
 ******************************************************************************/

namespace default_impl
{

namespace BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )
{

template< class Signature, class ResultPred >
struct impl;

template<
    class T, class Result, class ResultPred,
    bool = ::sake::boost_ext::is_builtin_object<
               typename ::sake::boost_ext::remove_reference<T>::type
           >::value
>
struct dispatch;

template< class T, class Result, class ResultPred >
struct dispatch< T, Result, ResultPred, true >
    : SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, Result, ResultPred )
{ };

template< class T, class Result, class ResultPred >
struct dispatch< T, Result, ResultPred, false >
    : impl< Result ( T ), ResultPred >
{ };

} // namespace BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )

template< class T, class Result, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )::dispatch< T, Result, ResultPred >
{ };

namespace BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )
{

#undef SAKE_INTROSPECTION_TRAIT_NAME
#define SAKE_INTROSPECTION_TRAIT_NAME impl
#define SAKE_INTROSPECTION_FUNCTION_DECLARE( Result, T_tuple ) \
    SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, SAKE_BOOST_EXT_PP_TUPLE_REM1 T_tuple )
#define SAKE_INTROSPECTION_FUNCTION_APPLY( x_tuple ) \
    SAKE_INTROSPECTION_OPERATOR_APPLY x_tuple
#define SAKE_INTROSPECTION_FUNCTION_ARITY 1
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

} // namespace BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )

} // namespace default_impl



#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_OPERATOR_NAME
#undef SAKE_INTROSPECTION_OPERATOR_DECLARE
#undef SAKE_INTROSPECTION_OPERATOR_APPLY
#undef SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT
#undef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR
