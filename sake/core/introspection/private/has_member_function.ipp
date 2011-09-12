/*******************************************************************************
 * core/introspection/private/has_member_function.ipp
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
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultMetafunction ) boost::false_type
#endif // #ifndef SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION

#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 0, SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY )
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
    class ResultMetafunction = ::boost::mpl::always< ::boost::true_type >
>
struct SAKE_INTROSPECTION_TRAIT_NAME;



namespace no_ext
{

namespace trait_name_private
{

template<
    class T,
    class Signature = void,
    class ResultMetafunction = ::boost::mpl::always< ::boost::true_type >,
    bool = ::sake::boost_ext::is_builtin_object<
        typename ::sake::boost_ext::remove_reference<T>::type
    >::value
>
struct dispatch;

template< class T, class Signature, class ResultMetafunction >
struct dispatch< T, Signature, ResultMetafunction, true >
    : SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultMetafunction )
{ };

struct base
#if min_arity == 1 && max_arity == 1
{ void SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME (int) { } };
#else // #if min_arity == 1 && max_arity == 1
{ void SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME ( ) { } };
#endif // #if min_arity == 1 && max_arity == 1

#if min_arity == 1 && max_arity == 1
template< void (base::*)( int ) >
#else // #if min_arity == 1 && max_arity == 1
template< void (base::*)( ) >
#endif // #if min_arity == 1 && max_arity == 1
struct sfinae
{ typedef ::sake::no_type type; };

template< class T >
typename sfinae< &T::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >::type
test(int);
template< class T >
::sake::yes_type
test(...);

template< class T >
struct dispatch< T, void, ::boost::mpl::always< ::boost::true_type >, false >
{
private:
    struct derived
        : ::sake::boost_ext::remove_reference<T>::type,
          base
    { };
public:
    static const bool value =
        sizeof( ::sake::yes_type ) == sizeof( trait_name_private::test< derived >(0) );
    typedef dispatch type;
};

#if min_arity == 0

template<
    class T, class Result, class ResultMetafunction,
    bool = ::boost::mpl::apply1< ResultMetafunction, Result >::type::value
>
struct has_nullary;

// We would like this to just determine if
//     Result r = x.f();
// is syntactically valid, but that's not possible (using the same machinery as
// above, at least).  So we'll have to settle for pretty much an exact match of
// the member function signature.  At the very least, we'll explore various
// combinations of const qualifiers on the class type and const/reference
// qualifiers on the result type.
template< class T, class Result, class ResultMetafunction >
struct dispatch< T, Result ( ), ResultMetafunction, false >
    : boost::mpl::and_<
          dispatch<T>,
          ::boost::mpl::or_<
              has_nullary<
                  T,
                  Result,
                  ResultMetafunction
              >,
              has_nullary<
                  T,
                  typename ::sake::boost_ext::add_reference< Result >::type,
                  ResultMetafunction
              >,
              has_nullary<
                  T,
                  typename ::sake::boost_ext::add_reference_add_const< Result >::type,
                  ResultMetafunction
              >
          >
      >
{ };

template< class T, class Result >
class has_nullary_helper
{
    typedef typename ::boost::mpl::if_<
        ::boost::is_const<T>,
        Result (T::*)( ) const,
        Result (T::*)( )
    >::type member_function_ptr_type;
    template< member_function_ptr_type >
    struct sfinae
    { typedef ::sake::yes_type type; };
    template< class U >
    static typename sfinae< &U::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME >::type
    test(int);
    template< class U >
    static ::sake::no_type
    test(...);
public:
    static const bool value = sizeof( ::sake::yes_type ) == sizeof( test<T>(0) );
    typedef has_nullary_helper type;
};

template< class T, class Result, class ResultMetafunction >
struct has_nullary< T, Result, ResultMetafunction, false >
    : ::boost::false_type
{ };

template< class T, class Result, class ResultMetafunction >
struct has_nullary< T, Result, ResultMetafunction, true >
    : ::boost::mpl::or_<
          has_nullary_helper< T, Result >,
          has_nullary_helper< T const, Result >
      >
{ };

template< class T, class Result, class ResultMetafunction >
struct has_nullary< T const, Result, ResultMetafunction, true >
    : has_nullary_helper< T const, Result >
{ };

template< class T, class Result, class ResultMetafunction >
struct has_nullary< T&, Result, ResultMetafunction, true >
    : has_nullary< T, Result, ResultMetafunction, true >
{ };

#endif // #if min_arity == 0

template< class T >
struct derived
    : ::sake::boost_ext::remove_reference<T>::type
{
    typedef typename ::sake::boost_ext::remove_reference<T>::type base_type;
    // A compiler error here concerning inaccessible overloads probably
    // indicates that a member function overload of the given name is private.
    // In this case, the only resolution is to explicitly extend the trait for
    // this class.
    using base_type::SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME;
    ::sake::introspection_private::dummy
#if min_arity == 1 && max_arity == 1
    SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME (::sake::convertible_from_any) const;
#else // #if min_arity == 1 && max_arity == 1
    SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME (...) const;
#endif // #if min_arity == 1 && max_arity == 1
};

template< class T, class Signature >
class has_void_result;
template< class T, class Signature, class ResultMetafunction >
class non_void_result_helper;

#if max_arity > 0
#define BOOST_PP_ITERATION_LIMITS ( (min_arity > 0 ? min_arity : 1), max_arity )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/private/has_member_function.ipp>
#include BOOST_PP_ITERATE()
#endif // #if max_arity > 0

} // namespace trait_name_private

template< class T, class Signature, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : trait_name_private::dispatch< T, Signature, ResultMetafunction >
{ };

} // namespace no_ext



SAKE_EXTENSION_CLASS( SAKE_INTROSPECTION_TRAIT_NAME, 3 )

template< class T, class Signature, class ResultMetafunction >
struct SAKE_INTROSPECTION_TRAIT_NAME
    : ext::SAKE_INTROSPECTION_TRAIT_NAME< T, Signature, ResultMetafunction >
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
#define derived_member_function_T0N \
    ::sake::declval< derived_ >(). SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME ( \
        BOOST_PP_ENUM_BINARY_PARAMS( N, ::sake::declval< T, >() BOOST_PP_INTERCEPT ) \
    )

template< class T, class Result, class_T0N, class ResultMetafunction >
struct dispatch< T, Result ( T0N ), ResultMetafunction, false >
    : ::boost::mpl::and_<
          dispatch<T>,
          ::boost::mpl::not_< has_void_result< T, void ( T0N ) > >,
          non_void_result_helper< T, Result ( T0N ), ResultMetafunction >
      >
{ };

template< class T, class_T0N, class ResultMetafunction >
struct dispatch< T, void ( T0N ), ResultMetafunction, false >
    : ::boost::mpl::and_<
          dispatch<T>,
          ::boost::mpl::eval_if<
              has_void_result< T, void ( T0N ) >,
              ::boost::mpl::apply1< ResultMetafunction, void >,
              non_void_result_helper< T, void ( T0N ), ResultMetafunction >
          >
      >
{ };

template< class T, class_T0N >
class has_void_result< T, void ( T0N ) >
{
    typedef typename ::sake::boost_ext::propagate_qualifiers< T, derived<T> >::type derived_;
public:
    static const bool value = SAKE_EXPR_IS_VOID( derived_member_function_T0N );
    typedef has_void_result type;
};

template< class T, class Result, class_T0N, class ResultMetafunction >
class non_void_result_helper< T, Result ( T0N ), ResultMetafunction >
{
    typedef typename ::sake::boost_ext::propagate_qualifiers< T, derived<T> >::type derived_;
public:
    static const bool value =
       !SAKE_EXPR_IS_CONVERTIBLE( derived_member_function_T0N, ::sake::introspection_private::dummy )
     && SAKE_EXPR_IS_CONVERTIBLE( derived_member_function_T0N, Result )
     && SAKE_EXPR_APPLY( ResultMetafunction, derived_member_function_T0N );
    typedef non_void_result_helper type;
};

#undef class_T0N
#undef T0N
#undef derived_member_function_T0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
