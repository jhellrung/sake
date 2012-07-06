/*******************************************************************************
 * sake/core/introspection/private/has_operator_binary.ipp
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
 && !defined( SAKE_INTROSPECTION_OPERATOR_APPLY ))
#error SAKE_INTROSPECTION_OPERATOR_NAME not defined.
#endif // #if !defined( ... ) && ...

#ifndef SAKE_INTROSPECTION_OPERATOR_DECLARE
#define SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, T, U ) \
    Result operator SAKE_INTROSPECTION_OPERATOR_NAME ( T, U );
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_DECLARE

#ifndef SAKE_INTROSPECTION_OPERATOR_APPLY
#define SAKE_INTROSPECTION_OPERATOR_APPLY( x, y ) \
    x SAKE_INTROSPECTION_OPERATOR_NAME y
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_APPLY

#ifndef SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ) void
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT

#ifndef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultPred ) ::boost::false_type
#endif // #ifndef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR



template<
    class T, class U = T,
    class Result = SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;

namespace extension
{
template< class T, class U, class Result, class ResultPred, class Enable = void >
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace extension

namespace default_impl
{
template<
    class T, class U,
    class Result = SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace default_impl



/*******************************************************************************
 * struct SAKE_INTROSPECTION_TRAIT_NAME< T, U, Result = ..., ResultPred = ... >
 ******************************************************************************/

template< class T, class U, class Result, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : extension::SAKE_INTROSPECTION_TRAIT_NAME< T, U, Result, ResultPred >
{ };

/*******************************************************************************
 * struct extension::SAKE_INTROSPECTION_TRAIT_NAME< T, U, Result, ResultPred, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class U, class Result, class ResultPred, class Enable /*= void*/ >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : default_impl::SAKE_INTROSPECTION_TRAIT_NAME< T, U, Result, ResultPred >
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::SAKE_INTROSPECTION_TRAIT_NAME< T, U, Result = ..., ResultPred = ... >
 ******************************************************************************/

namespace default_impl
{

#define binary_trait_name_private \
    BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )

namespace binary_trait_name_private
{

template< class Signature, class ResultPred >
struct impl;

template<
    class T, class U, class Result, class ResultPred,
    bool = ::sake::boost_ext::is_builtin_object<
               typename ::sake::boost_ext::remove_reference<T>::type >::value
        && ::sake::boost_ext::is_builtin_object<
               typename ::sake::boost_ext::remove_reference<U>::type >::value
>
struct dispatch;

template< class T, class U, class Result, class ResultPred >
struct dispatch< T, U, Result, ResultPred, true >
    : SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultPred )
{ };

template< class T, class U, class Result, class ResultPred >
struct dispatch< T, U, Result, ResultPred, false >
    : binary_trait_name_private::impl< Result ( T, U ), ResultPred >
{ };

} // namespace binary_trait_name_private

template< class T, class U, class Result, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : binary_trait_name_private::dispatch< T, U, Result, ResultPred >
{ };

namespace binary_trait_name_private
{

#undef SAKE_INTROSPECTION_TRAIT_NAME
#define SAKE_INTROSPECTION_TRAIT_NAME impl
#define SAKE_INTROSPECTION_FUNCTION_DECLARE( Result, T_tuple ) \
    SAKE_INTROSPECTION_OPERATOR_DECLARE( \
        Result, \
        BOOST_PP_TUPLE_ELEM( 2, 0, T_tuple ), \
        BOOST_PP_TUPLE_ELEM( 2, 1, T_tuple ) \
    )
#define SAKE_INTROSPECTION_FUNCTION_APPLY( x_tuple ) \
    SAKE_INTROSPECTION_OPERATOR_APPLY x_tuple
#define SAKE_INTROSPECTION_FUNCTION_ARITY 2
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()

} // namespace binary_trait_name_private

} // namespace default_impl



#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_OPERATOR_NAME
#undef SAKE_INTROSPECTION_OPERATOR_DECLARE
#undef SAKE_INTROSPECTION_OPERATOR_APPLY
#undef SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT
#undef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR
