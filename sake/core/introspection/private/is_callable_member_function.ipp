/*******************************************************************************
 * sake/core/introspection/private/is_callable_member_function.ipp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_INTROSPECTION_TRAIT_NAME
#error SAKE_INTROSPECTION_TRAIT_NAME not defined.
#endif // #ifndef SAKE_INTROSPECTION_TRAIT_NAME

#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
#error SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME not defined.
#endif // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME

#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ) void
#endif // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE

#ifndef SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred ) ::boost::false_type
#endif // #ifndef SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION

#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, SAKE_INTROSPECTION_DEFAULT_MAX_ARITY )
#endif // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS

#define min_arity BOOST_PP_TUPLE_ELEM( 2, 0, SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS )
#define max_arity BOOST_PP_TUPLE_ELEM( 2, 1, SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS )
#if !(0 <= min_arity && min_arity <= max_arity)
#error Invalid SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS.
#endif // #if !(...)



#define trait_name_private BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )



template<
    class T,
    class Signature = SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;



namespace no_ext
{

namespace trait_name_private
{

typedef ::boost::mpl::always< ::boost::true_type > always_true;

template<
    class T,
    class Signature = void,
    class ResultPred = always_true,
    bool = ::sake::boost_ext::is_builtin_object<
        typename ::sake::boost_ext::remove_reference<T>::type
    >::value
>
struct dispatch;

template< class T, class Signature, class ResultPred >
struct dispatch< T, Signature, ResultPred, true >
    : SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred )
{ };

struct member_detector_base
{
#if min_arity == max_arity
    void SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
        ( BOOST_PP_ENUM_PARAMS( min_arity, int BOOST_PP_INTERCEPT ) )
    { }
#else // #if min_arity == max_arity
    void SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME ( ) { }
#endif // #if min_arity == max_arity
};

template<
    void (member_detector_base::*)
#if min_arity == max_arity
        ( BOOST_PP_ENUM_PARAMS( min_arity, int BOOST_PP_INTERCEPT ) )
#else // #if min_arity == max_arity
        ( )
#endif // #if min_arity == max_arity
>
struct sfinae_member;

template< class T >
::sake::no_tag test_member(sfinae_member< &T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >*);
template< class T >
::sake::yes_tag test_member(...);

template< class T >
class has_member
{
    struct detector : T, member_detector_base { };
public:
    static bool const value = sizeof( ::sake::yes_tag ) == sizeof( test_member< detector >(0) );
    typedef has_member type;
};

#if SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_OPERATOR( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME )

template< class T >
struct dispatch< T, void, always_true, false >
    : has_member<T>
{ };

#else // #if SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_OPERATOR( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME )

template< class > struct sfinae_member_type;
template< class T >
::sake::yes_tag test_member_type(sfinae_member_type< typename T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >*);
template< class T >
::sake::no_tag test_member_type(...);

template< class T >
struct not_has_member_type
{
    static bool const value = sizeof( ::sake::no_tag ) == sizeof( test_member_type<T>(0) );
    typedef not_has_member_type type;
};

template< class T >
struct dispatch< T, void, always_true, false >
    : ::sake::boost_ext::mpl::and2< has_member<T>, not_has_member_type<T> >
{ };

#endif // #if SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_OPERATOR( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME )

template< class T > struct dispatch< T const, void, always_true, false > : dispatch<T> { };
template< class T > struct dispatch< T&, void, always_true, false > : dispatch<T> { };

#if min_arity == 0

template<
    class T, class Result, class ResultPred, class LiteralResult,
    bool = ::sake::boost_ext::mpl::and2<
               ::sake::boost_ext::is_convertible< LiteralResult, Result >,
               ::boost::mpl::apply1< ResultPred, Result >
           >::value
>
struct has_nullary;

// We would like this to just determine if
//     Result r = x.f();
// is syntactically valid, but that's not possible (using the same machinery as
// above, at least).  So we'll have to settle for pretty much an exact match of
// the member function signature.  At the very least, we'll explore various
// combinations of const qualifiers on the class type and const/reference
// qualifiers on the result type.
template< class T, class Result, class ResultPred >
struct dispatch< T, Result ( ), ResultPred, false >
    : ::sake::boost_ext::mpl::and2<
          dispatch<T>,
          ::sake::boost_ext::mpl::or3<
              has_nullary<
                  T, Result, ResultPred,
                  typename ::sake::boost_ext::remove_qualifiers< Result >::type
              >,
              has_nullary<
                  T, Result, ResultPred,
                  typename ::sake::boost_ext::add_reference<
                      typename ::sake::boost_ext::remove_qualifiers< Result >::type
                  >::type
              >,
              has_nullary<
                  T, Result, ResultPred,
                  typename ::sake::boost_ext::add_reference_add_const<
                      typename ::sake::boost_ext::remove_qualifiers< Result >::type
                  >::type
              >
          >
      >
{ };

template< class T, class Result, class ResultPred >
struct dispatch< T&, Result ( ), ResultPred, false >
    : dispatch< T, Result ( ), ResultPred, false >
{ };

template< class T, class Nullary >
class has_nullary_helper
{
    template< Nullary > struct sfinae;
    template< class U > static ::sake::yes_tag test(sfinae< &U::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >*);
    template< class U > static ::sake::no_tag test(...);
public:
    // A compiler error here concerning an inaccessible private member indicates
    // that a member function overload of the given name is private.  In this
    // case, the only resolution is to explicitly extend the trait for this
    // class.
    static bool const value = sizeof( ::sake::yes_tag ) == sizeof( test<T>(0) );
    typedef has_nullary_helper type;
};

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary< T, Result, ResultPred, LiteralResult, false >
    : ::boost::false_type
{ };

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary< T, Result, ResultPred, LiteralResult, true >
    : ::sake::boost_ext::mpl::or2<
          has_nullary_helper< T, LiteralResult (T::*)( ) >,
          has_nullary_helper< T const, LiteralResult (T::*)( ) const >
      >
{ };

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary< T const, Result, ResultPred, LiteralResult, true >
    : has_nullary_helper< T const, LiteralResult (T::*)( ) const >
{ };

#endif // #if min_arity == 0

template< class T >
struct fallback : T
{
    // A compiler error here concerning inaccessible overloads or private member
    // indicates that a member function overload of the given name is private.
    // In this case, the only resolution is to explicitly extend the trait for
    // this class.
    using T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME;
    ::sake::introspection_private::dummy
    SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
#if min_arity == max_arity
        ( BOOST_PP_ENUM_PARAMS(
            min_arity,
            ::sake::convertible_from_any BOOST_PP_INTERCEPT
        ) ) const;
#else // #if min_arity == max_arity
        (...) const;
#endif // #if min_arity == max_arity
};

template< class T > struct fallback< T const > : fallback<T> { };
template< class T > struct fallback< T& > : fallback<T> { };

template< class T, class Signature >
class has_void_result;
template< class T, class Signature, class ResultPred >
class non_void_result_helper;

#if min_arity != max_arity && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class T, class Result, class T0, class... T, class ResultPred >
struct dispatch< T, Result ( T0, T... ), ResultPred, false >
    : ::sake::boost_ext::mpl::and3<
          dispatch<T>,
          ::boost::mpl::not_< has_void_result< T, void ( T0, T... ) > >,
          non_void_result_helper< T, Result ( T0, T... ), ResultPred >
      >
{ };

template< class T, class T0, class... T, class ResultPred >
struct dispatch< T, void ( T0, T... ), ResultPred, false >
    : ::sake::boost_ext::mpl::and2<
          dispatch<T>,
          ::boost::mpl::eval_if<
              has_void_result< T, void ( T0, T... ) >,
              ::boost::mpl::apply1< ResultPred, void >,
              non_void_result_helper< T, void ( T0, T... ), ResultPred >
          >
      >
{ };

template< class T, class T0, class... T >
class has_void_result< T, void ( T0, T... ) >
{
    typedef typename ::sake::boost_ext::propagate_qualifiers< T, fallback<T> >::type fallback_;
public:
    static bool const value = SAKE_EXPR_IS_VOID(
        ::sake::declval< fallback_ >().
            SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
                ( ::sake::declval< T0 >(), ::sake::declval<T>()... )
    );
    typedef has_void_result type;
};

template< class T, class Result, class T0, class... T, class ResultPred >
class non_void_result_helper< T, Result ( T0, T... ), ResultPred >
{
    typedef typename ::sake::boost_ext::propagate_qualifiers< T, fallback<T> >::type fallback_;
public:
    static bool const value =
       !SAKE_EXPR_IS_CONVERTIBLE(
            ::sake::declval< fallback_ >().
                SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
                    ( ::sake::declval< T0 >(), ::sake::declval<T>()... ),
            ::sake::introspection_private::dummy
        )
     && SAKE_EXPR_IS_CONVERTIBLE(
            ::sake::declval< fallback_ >().
                SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
                    ( ::sake::declval< T0 >(), ::sake::declval<T>()... ),
            Result
        )
     && SAKE_EXPR_APPLY(
            ResultPred,
            ::sake::declval< fallback_ >().
                SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
                    ( ::sake::declval< T0 >(), ::sake::declval<T>()... ),
        );
    typedef non_void_result_helper type;
};

#else // #if min_arity != max_arity && !defined( BOOST_NO_VARIADIC_TEMPLATES )

#if max_arity > 0
#define BOOST_PP_ITERATION_LIMITS ( (min_arity > 0 ? min_arity : 1), max_arity )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/private/is_callable_member_function.ipp>
#include BOOST_PP_ITERATE()
#endif // #if max_arity > 0

#endif // #if min_arity != max_arity && !defined( BOOST_NO_VARIADIC_TEMPLATES )

} // namespace trait_name_private

template< class T, class Signature, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : trait_name_private::dispatch< T, Signature, ResultPred >
{ };

} // namespace no_ext



SAKE_EXTENSION_CLASS( SAKE_INTROSPECTION_TRAIT_NAME, 3 )

template< class T, class Signature, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : ext::SAKE_INTROSPECTION_TRAIT_NAME< T, Signature, ResultPred >
{ };



#undef min_arity
#undef max_arity
#undef trait_name_private

#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#undef SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define class_T0N   BOOST_PP_ENUM_PARAMS( N, class T )
#define T0N         BOOST_PP_ENUM_PARAMS( N, T )
#define fallback_member_T0N \
    ::sake::declval< fallback_ >(). SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME ( \
        BOOST_PP_ENUM_BINARY_PARAMS( N, ::sake::declval< T, >() BOOST_PP_INTERCEPT ) \
    )

template< class T, class Result, class_T0N, class ResultPred >
struct dispatch< T, Result ( T0N ), ResultPred, false >
    : ::sake::boost_ext::mpl::and3<
          dispatch<T>,
          ::boost::mpl::not_< has_void_result< T, void ( T0N ) > >,
          non_void_result_helper< T, Result ( T0N ), ResultPred >
      >
{ };

template< class T, class_T0N, class ResultPred >
struct dispatch< T, void ( T0N ), ResultPred, false >
    : ::sake::boost_ext::mpl::and2<
          dispatch<T>,
          ::boost::mpl::eval_if<
              has_void_result< T, void ( T0N ) >,
              ::boost::mpl::apply1< ResultPred, void >,
              non_void_result_helper< T, void ( T0N ), ResultPred >
          >
      >
{ };

template< class T, class_T0N >
class has_void_result< T, void ( T0N ) >
{
    typedef typename ::sake::boost_ext::propagate_qualifiers< T, fallback<T> >::type fallback_;
public:
    static bool const value = SAKE_EXPR_IS_VOID( fallback_member_T0N );
    typedef has_void_result type;
};

template< class T, class Result, class_T0N, class ResultPred >
class non_void_result_helper< T, Result ( T0N ), ResultPred >
{
    typedef typename ::sake::boost_ext::propagate_qualifiers< T, fallback<T> >::type fallback_;
public:
    static bool const value =
       !SAKE_EXPR_IS_CONVERTIBLE( fallback_member_T0N, ::sake::introspection_private::dummy )
     && SAKE_EXPR_IS_CONVERTIBLE( fallback_member_T0N, Result )
     && SAKE_EXPR_APPLY( ResultPred, fallback_member_T0N );
    typedef non_void_result_helper type;
};

#undef class_T0N
#undef T0N
#undef fallback_member_T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
