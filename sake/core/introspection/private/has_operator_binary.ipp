/*******************************************************************************
 * core/introspection/private/has_operator_binary.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_INTROSPECTION_TRAIT_NAME
#error SAKE_INTROSPECTION_TRAIT_NAME not defined.
#endif // #ifndef SAKE_INTROSPECTION_TRAIT_NAME

#ifndef SAKE_INTROSPECTION_OPERATOR_DECLARE
#error SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, T, U ) not defined.
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_DECLARE

#ifndef SAKE_INTROSPECTION_OPERATOR_APPLY
#error SAKE_INTROSPECTION_OPERATOR_APPLY( x, y ) not defined.
#endif // #ifndef SAKE_INTROSPECTION_OPERATOR_APPLY

#ifndef SAKE_INTROSPECTION_DEFAULT_RESULT
#define SAKE_INTROSPECTION_DEFAULT_RESULT( T, U ) void
#endif // #ifndef SAKE_INTROSPECTION_DEFAULT_RESULT

#ifndef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultMetafunction ) ::boost::false_type
#endif // #ifndef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR



#define trait_name_private        BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )
#define operator_apply_expression SAKE_INTROSPECTION_OPERATOR_APPLY( ::sake::declval<T>(), ::sake::declval<U>() )



template<
    class T, class U,
    class Result = SAKE_INTROSPECTION_DEFAULT_RESULT( T, U ),
    class ResultMetafunction = ::boost::mpl::always< ::boost::true_type >
> struct SAKE_INTROSPECTION_TRAIT_NAME;



namespace no_ext
{

namespace trait_name_private
{

SAKE_INTROSPECTION_OPERATOR_DECLARE(
    ::sake::introspection_private::dummy,
    ::sake::convertible_from_any,
    ::sake::convertible_from_any
)

template<
    class T, class U, class Result, class ResultMetafunction,
    bool = ::sake::boost_ext::is_builtin_object<
               typename ::sake::boost_ext::remove_reference<T>::type
           >::value &&
           ::sake::boost_ext::is_builtin_object<
               typename ::sake::boost_ext::remove_reference<U>::type
           >::value
>
struct dispatch;

template< class T, class U, class Result, class ResultMetafunction >
struct dispatch< T, U, Result, ResultMetafunction, true >
    : SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultMetafunction )
{ };

template< class T, class U >
struct has_void_result
{
    static const bool value = SAKE_EXPR_IS_VOID( operator_apply_expression );
    typedef has_void_result type;
};

template< class T, class U, class Result, class ResultMetafunction >
struct non_void_result_helper
{
    static const bool value =
       !SAKE_EXPR_IS_CONVERTIBLE( operator_apply_expression, ::sake::introspection_private::dummy )
     && SAKE_EXPR_IS_CONVERTIBLE( operator_apply_expression, Result )
     && SAKE_EXPR_APPLY( ResultMetafunction, operator_apply_expression );
    typedef non_void_result_helper type;
};

template< class T, class U, class Result, class ResultMetafunction >
struct dispatch< T, U, Result, ResultMetafunction, false >
    : ::boost::mpl::and_<
          ::boost::mpl::not_< trait_name_private::has_void_result<T,U> >,
          trait_name_private::non_void_result_helper< T, U, Result, ResultMetafunction >
      >
{ };

template< class T, class U, class ResultMetafunction >
struct dispatch< T, U, void, ResultMetafunction, false >
    : ::boost::mpl::eval_if<
          trait_name_private::has_void_result<T,U>,
          ::boost::mpl::apply1< ResultMetafunction, void >,
          trait_name_private::non_void_result_helper< T, U, void, ResultMetafunction >
      >
{ };

} // namespace trait_name_private

template< class T, class U, class Result, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : trait_name_private::dispatch< T, U, Result, ResultMetafunction >
{ };

} // namespace no_ext



SAKE_EXTENSION_CLASS_N( SAKE_INTROSPECTION_TRAIT_NAME, 4, 2 )

template< class T, class U, class Result, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : ext:: BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, 0 ) < T, U, Result, ResultMetafunction >
{ };



#undef trait_name_private
#undef operator_apply_expression



#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_OPERATOR_DECLARE
#undef SAKE_INTROSPECTION_OPERATOR_APPLY
#undef SAKE_INTROSPECTION_DEFAULT_RESULT
#undef SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR
