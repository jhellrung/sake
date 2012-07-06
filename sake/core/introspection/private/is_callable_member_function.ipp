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

#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#define min_arity 0
#define max_arity SAKE_INTROSPECTION_DEFAULT_MAX_ARITY
#else // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#define min_arity BOOST_PP_TUPLE_ELEM( 2, 0, SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS )
#define max_arity BOOST_PP_TUPLE_ELEM( 2, 1, SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS )
#if !(0 <= min_arity && min_arity <= max_arity)
#error Invalid SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS.
#endif // #if !(1 <= min_arity && min_arity <= max_arity)
#endif // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#else // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
#ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#error Must not define both SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY and\
 SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#endif // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#define min_arity SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
#define max_arity SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
#endif // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY



template<
    class T,
    class Signature = SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;

namespace extension
{
template< class T, class Signature, class ResultPred, class Enable = void >
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace extension

namespace default_impl
{
template<
    class T,
    class Signature = SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T ),
    class ResultPred = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;
} // namespace default_impl



/*******************************************************************************
 * struct SAKE_INTROSPECTION_TRAIT_NAME< T, Signature = ..., ResultPred = ... >
 ******************************************************************************/

template< class T, class Signature, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : extension::SAKE_INTROSPECTION_TRAIT_NAME< T, Signature, ResultPred >
{ };

/*******************************************************************************
 * struct extension::SAKE_INTROSPECTION_TRAIT_NAME< T, Signature, ResultPred, Enable = void >
 ******************************************************************************/

namespace extension
{

template< class T, class Signature, class ResultPred, class Enable /*= void*/ >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : default_impl::SAKE_INTROSPECTION_TRAIT_NAME< T, Signature, ResultPred >
{ };

} // namespace extension

/*******************************************************************************
 * struct default_impl::SAKE_INTROSPECTION_TRAIT_NAME< T, Signature = ..., ResultPred = ... >
 ******************************************************************************/

namespace default_impl
{

#define trait_name_private \
    BOOST_PP_CAT( SAKE_INTROSPECTION_TRAIT_NAME, _private )

namespace trait_name_private
{

typedef ::boost::mpl::always< ::boost::true_type > always_true;

template<
    class T,
    class Signature = void,
    class ResultPred = trait_name_private::always_true,
    bool = ::sake::boost_ext::is_builtin_object<
        typename ::sake::boost_ext::remove_reference<T>::type >::value
>
struct dispatch;

template< class T, class Signature, class ResultPred >
struct dispatch< T, Signature, ResultPred, true >
    : SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred )
{ };

struct member_detector_base
{
#ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
    void SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
        ( BOOST_PP_ENUM_PARAMS( SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY, int BOOST_PP_INTERCEPT ) )
    { }
#else // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
    void SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME ( ) { }
#endif // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
};

template<
    void (member_detector_base::*)
#ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
        ( BOOST_PP_ENUM_PARAMS( SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY, int BOOST_PP_INTERCEPT ) )
#else // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
        ( )
#endif // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
>
struct sfinae_member;

template< class T >
::sake::false_tag
test_member(sfinae_member< &T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >*);
template< class T >
::sake::true_tag
test_member(...);

template< class T >
class has_member
{
    struct detector : T, trait_name_private::member_detector_base { };
public:
    static bool const value =
        SAKE_SIZEOF_TRUE_TAG
     == sizeof( trait_name_private::test_member< detector >(0) );
    typedef has_member type;
};

#if SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_OPERATOR( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME )

template< class T >
struct dispatch< T, void, trait_name_private::always_true, false >
    : trait_name_private::has_member<T>
{ };

#else // #if SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_OPERATOR( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME )

template< class > struct sfinae_member_type;
template< class T >
::sake::true_tag
test_member_type(trait_name_private::sfinae_member_type<
    typename T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >*);
template< class T >
::sake::false_tag
test_member_type(...);

template< class T >
struct not_has_member_type
{
    static bool const value =
        SAKE_SIZEOF_FALSE_TAG
     == sizeof( trait_name_private::test_member_type<T>(0) );
    typedef not_has_member_type type;
};

template< class T >
struct dispatch< T, void, trait_name_private::always_true, false >
    : ::sake::boost_ext::mpl::and2<
          trait_name_private::has_member<T>,
          trait_name_private::not_has_member_type<T>
      >
{ };

#endif // #if SAKE_BOOST_EXT_PP_KEYWORD_HAS_PREFIX_OPERATOR( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME )

template< class T >
struct dispatch< T const, void, trait_name_private::always_true, false >
    : trait_name_private::dispatch<T>
{ };
template< class T >
struct dispatch< T&, void, trait_name_private::always_true, false >
    : trait_name_private::dispatch<T>
{ };

template< class T >
struct fallback : T
{
    typedef fallback type;
    // A compiler error here concerning inaccessible overloads or private
    // members indicates that a member function overload of the given name is
    // private. In this case, the only resolution is to explicitly extend the
    // trait for this class.
    using T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME;
    ::sake::introspection_private::dummy
    SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME(
#ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
        BOOST_PP_ENUM_PARAMS( SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY,
            ::sake::convertible_from_any BOOST_PP_INTERCEPT )
#else // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
        ...
#endif // #ifdef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
        // const qualifier ensures this overload is always considered regardless
        // of the const qualification of target object.
        // volatile qualifier is key to avoid overload ambiguity.
        ) const volatile;
};

template< class T >
struct fallback< T const >
{ typedef trait_name_private::fallback<T> const type; };
template< class T >
struct fallback< T& >
{
    BOOST_STATIC_ASSERT((!::sake::boost_ext::is_rvalue_reference< T& >::value));
    typedef typename trait_name_private::fallback<T>::type & type;
};

template< class T, class Signature >
class has_non_void_result;
template< class T, class Signature, class ResultPred >
class check_non_void_result;

#if min_arity == 0

// The machinery for general arity member functions fails specifically for
// nullary static member functions, so we go out of our way to support them as
// best we can.

template<
    class T, class Result, class ResultPred,
    class LiteralResult = Result,
    bool = ::sake::boost_ext::mpl::and2<
               ::sake::boost_ext::is_convertible< LiteralResult, Result >,
               ::boost::mpl::apply1< ResultPred, LiteralResult >
           >::value
>
struct has_nullary_static_literal;

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary_static_literal< T, Result, ResultPred, LiteralResult, false >
    : ::boost::false_type
{ };

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary_static_literal< T, Result, ResultPred, LiteralResult, true >
{
    template< LiteralResult (*)( ) > struct sfinae;
    template< class T_ >
    static ::sake::true_tag
    test(sfinae< &T_::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >*);
    template< class T_ >
    static ::sake::false_tag
    test(...);
public:
    // A compiler error here concerning an inaccessible private member indicates
    // that a member function overload of the given name is private.  In this
    // case, the only resolution is to explicitly extend the trait for this
    // class.
    static bool const value = SAKE_SIZEOF_TRUE_TAG == sizeof( test<T>(0) );
    typedef has_nullary_static_literal type;
};

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary_static_literal<
    T const, Result, ResultPred, LiteralResult, true >
    : trait_name_private::has_nullary_static_literal<
          T, Result, ResultPred, LiteralResult >
{ };

template< class T, class Result, class ResultPred, class LiteralResult >
struct has_nullary_static_literal<
    T&, Result, ResultPred, LiteralResult, true >
    : trait_name_private::has_nullary_static_literal<
          T, Result, ResultPred, LiteralResult >
{ };

template< class T, class Result, class ResultPred >
struct has_nullary_static
{
private:
    typedef typename ::sake::boost_ext::remove_qualifiers<
        Result >::type noqual_result_type;
public:
    static bool const value = ::sake::boost_ext::mpl::or3<
        trait_name_private::has_nullary_static_literal<
            T, Result, ResultPred, noqual_result_type >,
        trait_name_private::has_nullary_static_literal<
            T, Result, ResultPred, noqual_result_type& >,
        trait_name_private::has_nullary_static_literal<
            T, Result, ResultPred, noqual_result_type const & >
    >::value;
    typedef has_nullary_static type;
};

template< class T, class Result, class ResultPred >
struct has_nullary_static< T const, Result, ResultPred >
    : trait_name_private::has_nullary_static< T, Result, ResultPred >
{ };

template< class T, class Result, class ResultPred >
struct has_nullary_static< T&, Result, ResultPred >
    : trait_name_private::has_nullary_static< T, Result, ResultPred >
{ };

template< class T, class Result, class ResultPred >
struct dispatch< T, Result ( ), ResultPred, false >
    : ::sake::boost_ext::mpl::and2<
          trait_name_private::dispatch<T>,
          ::sake::boost_ext::mpl::or2<
              trait_name_private::has_nullary_static<
                  T, Result, ResultPred >,
              ::sake::boost_ext::mpl::and2<
                  trait_name_private::has_non_void_result<
                      T, void ( ) >,
                  trait_name_private::check_non_void_result<
                      T, Result ( ), ResultPred >
              >
          >
      >
{ };

template< class T, class ResultPred >
struct dispatch< T, void ( ), ResultPred, false >
    : ::sake::boost_ext::mpl::and2<
          trait_name_private::dispatch<T>,
          ::sake::boost_ext::mpl::or2<
              trait_name_private::has_nullary_static_literal<
                  T, void, ResultPred, void >,
              ::boost::mpl::eval_if<
                  trait_name_private::has_non_void_result<
                      T, void ( ) >,
                  trait_name_private::check_non_void_result<
                      T, void ( ), ResultPred >,
                  ::boost::mpl::apply1< ResultPred, void >
              >
          >
      >
{ };

#endif // #if min_arity == 0

#if !defined( SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY ) \
 && !defined( BOOST_NO_VARIADIC_TEMPLATES )

template< class T, class Result, class U0, class... U, class ResultPred >
struct dispatch< T, Result ( U0, U... ), ResultPred, false >
#if defined( __GNUC__ ) \
 && defined( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME_IS_OPERATOR_ASSIGN )
    : ::sake::boost_ext::mpl::and2<
#else // #if defined( __GNUC__ ) ## defined( ... )
    : ::sake::boost_ext::mpl::and3<
          trait_name_private::dispatch<T>,
#endif // #if defined( __GNUC__ ) ## defined( ... )
          trait_name_private::has_non_void_result<
              T, void ( U0, U... ) >,
          trait_name_private::check_non_void_result<
              T, Result ( U0, U... ), ResultPred >
      >
{ };

template< class T, class U0, class... U, class ResultPred >
struct dispatch< T, void ( U0, U... ), ResultPred, false >
#if defined( __GNUC__ ) \
 && defined( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME_IS_OPERATOR_ASSIGN )
    : ::sake::boost_ext::mpl::and1<
#else // #if defined( __GNUC__ ) ## defined( ... )
    : ::sake::boost_ext::mpl::and2<
          trait_name_private::dispatch<T>,
#endif // #if defined( __GNUC__ ) ## defined( ... )
          ::boost::mpl::eval_if<
              trait_name_private::has_non_void_result<
                  T, void ( U0, U... ) >,
              trait_name_private::check_non_void_result<
                  T, void ( U0, U... ), ResultPred >,
              ::boost::mpl::apply1< ResultPred, void >
          >
      >
{ };

#define fallback_member_U \
    ::sake::declval< fallback_ >(). \
        SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME(::sake::declval<U>()...)

template< class T, class... U >
class has_non_void_result< T, void ( U... ) >
{
    typedef typename trait_name_private::fallback<T>::type fallback_;
public:
    static bool const value = !SAKE_EXPR_IS_VOID( fallback_member_U );
    typedef has_non_void_result type;
};

template< class T, class Result, class... U, class ResultPred >
class check_non_void_result< T, Result ( U... ), ResultPred >
{
    typedef typename trait_name_private::fallback<T>::type fallback_;
    typedef ::sake::introspection_private::check_result<
        Result,
        typename ::boost::mpl::lambda< ResultPred >::type
    > check_result_;
public:
    static bool const value =
        SAKE_EXPR_APPLY( check_result_, fallback_member_U );
    typedef check_non_void_result type;
};

#undef fallback_member_U

#else // #if !defined( ... ) && !defined( ... )

#define BOOST_PP_ITERATION_LIMITS ( min_arity, max_arity )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/private/is_callable_member_function.ipp>
#include BOOST_PP_ITERATE()

#endif // #if !defined( ... ) && !defined( ... )

} // namespace trait_name_private

template< class T, class Signature, class ResultPred >
struct SAKE_INTROSPECTION_TRAIT_NAME
{
    static bool const value =
        trait_name_private::dispatch< T, Signature, ResultPred >::value;
    typedef SAKE_INTROSPECTION_TRAIT_NAME type;
};

#undef trait_name_private

} // namespace default_impl



#undef min_arity
#undef max_arity

#undef SAKE_INTROSPECTION_TRAIT_NAME
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME_IS_OPERATOR_ASSIGN
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
#undef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#undef SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )
#define fallback_member_U0N \
    ::sake::declval< fallback_ >(). SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME ( \
        BOOST_PP_ENUM_BINARY_PARAMS( N, ::sake::declval< U, >() BOOST_PP_INTERCEPT ) )

#if N != 0

template< class T, class Result comma_class_U0N, class ResultPred >
struct dispatch< T, Result ( U0N ), ResultPred, false >
#if defined( __GNUC__ ) \
 && defined( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME_IS_OPERATOR_ASSIGN )
    : ::sake::boost_ext::mpl::and2<
#else // #if defined( __GNUC__ ) ## defined( ... )
    : ::sake::boost_ext::mpl::and3<
          trait_name_private::dispatch<T>,
#endif // #if defined( __GNUC__ ) ## defined( ... )
          trait_name_private::has_non_void_result<
              T, void ( U0N ) >,
          trait_name_private::check_non_void_result<
              T, Result ( U0N ), ResultPred >
      >
{ };

template< class T comma_class_U0N, class ResultPred >
struct dispatch< T, void ( U0N ), ResultPred, false >
#if defined( __GNUC__ ) \
 && defined( SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME_IS_OPERATOR_ASSIGN )
    : ::sake::boost_ext::mpl::and1<
#else // #if defined( __GNUC__ ) ## defined( ... )
    : ::sake::boost_ext::mpl::and2<
          trait_name_private::dispatch<T>,
#endif // #if defined( __GNUC__ ) ## defined( ... )
          ::boost::mpl::eval_if<
              trait_name_private::has_non_void_result<
                  T, void ( U0N ) >,
              trait_name_private::check_non_void_result<
                  T, void ( U0N ), ResultPred >,
              ::boost::mpl::apply1< ResultPred, void >
          >
      >
{ };

#endif // #if N != 0

template< class T comma_class_U0N >
class has_non_void_result< T, void ( U0N ) >
{
    typedef typename trait_name_private::fallback<T>::type fallback_;
public:
    static bool const value = !SAKE_EXPR_IS_VOID( fallback_member_U0N );
    typedef has_non_void_result type;
};

template< class T, class Result comma_class_U0N, class ResultPred >
class check_non_void_result< T, Result ( U0N ), ResultPred >
{
    typedef typename trait_name_private::fallback<T>::type fallback_;
    typedef ::sake::introspection_private::check_result<
        Result,
        typename boost::mpl::lambda< ResultPred >::type
    > check_result_;
public:
    static bool const value =
        SAKE_EXPR_APPLY( check_result_, fallback_member_U0N );
    typedef check_non_void_result type;
};

#undef comma_class_U0N
#undef U0N
#undef fallback_member_U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
