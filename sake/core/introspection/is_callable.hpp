/*******************************************************************************
 * sake/core/introspection/is_callable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_callable<
 *     T,
 *     Signature = void,
 *     ResultPred = boost::mpl::always< boost::true_type >
 * >
 *
 * This is a metafunction which determines if an object of type T is callable
 * with the given signature.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_HPP
#define SAKE_CORE_INTROSPECTION_IS_CALLABLE_HPP

#include <boost/config.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/fwd.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/introspection/private/check_result.hpp>
#include <sake/core/introspection/private/dummy.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/is_compatible_signature.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Signature, class ResultPred >
struct builtin_is_callable;

} // namespace introspection_private

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME operator()
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS \
    ( 1, SAKE_INTROSPECTION_DEFAULT_MAX_ARITY )
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred ) \
    ::sake::introspection_private::builtin_is_callable< T, Signature, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

namespace introspection_private
{

template< class T, class Signature, class ResultPred >
struct builtin_is_callable
    : boost::false_type
{ };

template< class T, class Signature, class ResultPred >
struct builtin_is_callable< T&, Signature, ResultPred >
    : builtin_is_callable< T*, Signature, ResultPred >
{ };

#ifndef BOOST_NO_VARIADIC_TEMPLATES

template< class R, class... T, class S, class... U, class ResultPred >
struct builtin_is_callable< R (*)( T... ), S ( U... ), ResultPred >
    : boost_ext::mpl::and2<
          sake::is_compatible_signature< R ( T... ), S ( U... ) >,
          boost::mpl::apply1< ResultPred, R >
      >
{ };

template< class R, class... T, class ResultPred >
struct builtin_is_callable< R (*)( T... ), void, ResultPred >
    : boost::mpl::apply1< ResultPred, R >::type
{ };

#else // #ifndef BOOST_NO_VARIADIC_TEMPLATES

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_INTROSPECTION_DEFAULT_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/is_callable.hpp>
#include BOOST_PP_ITERATE()

#endif // #ifndef BOOST_NO_VARIADIC_TEMPLATES

} // namespace introspection_private

namespace default_impl
{

namespace is_callable_private
{

// Determining if a class has a nullary member function generally requires the
// result to match (almost) exactly.  However, in the special case of
// operator(), we're able to include implicit conversions as well, hence this
// specialization.

template< class T >
struct fallback_nullary : T
{
    typedef introspection_private::dummy (*fallback_type)();
    operator fallback_type() const;
};

template< class T >
struct fallback_nullary< T const >
    : is_callable_private::fallback_nullary<T>
{ };
template< class T >
struct fallback_nullary< T& >
    : is_callable_private::fallback_nullary<T>
{ };

template< class T >
class has_void_result< T, void ( ) >
{
    typedef typename boost_ext::propagate_qualifiers<
        T, is_callable_private::fallback_nullary<T> >::type fallback_;
public:
    static bool const value = SAKE_EXPR_IS_VOID( sake::declval< fallback_ >()() );
    typedef has_void_result type;
};

template< class T, class Result, class ResultPred >
class non_void_result_helper< T, Result ( ), ResultPred >
{
    typedef typename boost_ext::propagate_qualifiers<
        T, is_callable_private::fallback_nullary<T> >::type fallback_;
    typedef introspection_private::check_result<
        Result,
        typename boost::mpl::lambda< ResultPred >::type
    > check_result_;
public:
    static bool const value =
        SAKE_EXPR_APPLY( check_result_, sake::declval< fallback_ >()() );
    typedef non_void_result_helper type;
};

template< class T, class Result, class ResultPred >
struct dispatch< T, Result ( ), ResultPred, false >
    : boost_ext::mpl::and3<
          is_callable_private::dispatch<T>,
          boost::mpl::not_< has_void_result<
              T, void ( ) > >,
          is_callable_private::non_void_result_helper<
              T, Result ( ), ResultPred >
      >
{ };

template< class T, class ResultPred >
struct dispatch< T, void ( ), ResultPred, false >
    : boost_ext::mpl::and2<
          is_callable_private::dispatch< T, void >,
          boost::mpl::eval_if<
              is_callable_private::has_void_result<
                  T, void ( ) >,
              boost::mpl::apply1< ResultPred, void >,
              is_callable_private::non_void_result_helper<
                  T, void ( ), ResultPred >
          >
      >
{ };

} // namespace is_callable_private

} // namespace default_impl

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_T0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class T )
#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define T0N             BOOST_PP_ENUM_PARAMS( N, T )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

template< class R comma_class_T0N, class S comma_class_U0N, class ResultPred >
struct builtin_is_callable< R (*)( T0N ), S ( U0N ), ResultPred >
    : boost_ext::mpl::and2<
          sake::is_compatible_signature< R ( T0N ), S ( U0N ) >,
          boost::mpl::apply1< ResultPred, R >
      >
{ };

template< class R comma_class_T0N, class ResultPred >
struct builtin_is_callable< R (*)( T0N ), void, ResultPred >
    : boost::mpl::apply1< ResultPred, R >::type
{ };

#undef comma_class_T0N
#undef comma_class_U0N
#undef T0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING
