/*******************************************************************************
 * core/introspection/is_callable.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct is_callable<
 *     T,
 *     Signature = void,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 *
 * This is a metafunction which determines if an object of type T is callable
 * with the given signature.
 ******************************************************************************/

#ifndef BOOST_PP_IS_ITERATING

#ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_HPP
#define SAKE_CORE_INTROSPECTION_IS_CALLABLE_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/private/dummy.hpp>
#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/is_compatible_signature.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Signature, class ResultMetafunction >
struct builtin_is_callable;

} // namespace introspection_private

#define SAKE_INTROSPECTION_TRAIT_NAME           is_callable
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME operator()
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS \
    ( 1, SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY )
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultMetafunction ) \
    ::sake::introspection_private::builtin_is_callable< T, Signature, ResultMetafunction >
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

namespace introspection_private
{

template< class T, class Signature, class ResultMetafunction >
struct builtin_is_callable
    : boost::false_type
{ };

template< class T, class Signature, class ResultMetafunction >
struct builtin_is_callable< T&, Signature, ResultMetafunction >
    : builtin_is_callable< T*, Signature, ResultMetafunction >
{ };

#define BOOST_PP_ITERATION_LIMITS ( 0, SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY )
#define BOOST_PP_FILENAME_1       <sake/core/introspection/is_callable.hpp>
#include BOOST_PP_ITERATE()

} // namespace introspection_private

namespace no_ext
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

template< class T > struct fallback_nullary< T const > : fallback<T> { };
template< class T > struct fallback_nullary< T& > : fallback<T> { };

template< class T >
class has_void_result< T, void ( ) >
{
    typedef typename boost_ext::propagate_qualifiers< T, fallback_nullary<T> >::type fallback_;
public:
    static const bool value = SAKE_EXPR_IS_VOID( sake::declval< fallback_ >()() );
    typedef has_void_result type;
};

template< class T, class Result, class ResultMetafunction >
class non_void_result_helper< T, Result ( ), ResultMetafunction >
{
    typedef typename boost_ext::propagate_qualifiers< T, fallback_nullary<T> >::type fallback_;
public:
    static const bool value =
       !SAKE_EXPR_IS_CONVERTIBLE( sake::declval< fallback_ >()(), introspection_private::dummy )
     && SAKE_EXPR_IS_CONVERTIBLE( sake::declval< fallback_ >()(), Result )
     && SAKE_EXPR_APPLY( ResultMetafunction, sake::declval< fallback_ >()() );
    typedef non_void_result_helper type;
};

template< class T, class Result, class ResultMetafunction >
struct dispatch< T, Result ( ), ResultMetafunction, false >
    : boost::mpl::and_<
          dispatch<T>,
          boost::mpl::not_< has_void_result< T, void ( ) > >,
          non_void_result_helper< T, Result ( ), ResultMetafunction >
      >
{ };

template< class T, class ResultMetafunction >
struct dispatch< T, void ( ), ResultMetafunction, false >
    : boost::mpl::and_<
          dispatch< T, void >,
          boost::mpl::eval_if<
              has_void_result< T, void ( ) >,
              boost::mpl::apply1< ResultMetafunction, void >,
              non_void_result_helper< T, void ( ), ResultMetafunction >
          >
      >
{ };

} // namespace is_callable_private

} // namespace no_ext

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_HPP

#else // #ifndef BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

#define comma_class_T0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class T )
#define comma_class_U0N BOOST_PP_ENUM_TRAILING_PARAMS( N, class U )
#define T0N             BOOST_PP_ENUM_PARAMS( N, T )
#define U0N             BOOST_PP_ENUM_PARAMS( N, U )

template< class R comma_class_T0N, class S comma_class_U0N, class ResultMetafunction >
struct builtin_is_callable< R (*)( T0N ), S ( U0N ), ResultMetafunction >
    : boost::mpl::and_<
          sake::is_compatible_signature< R ( T0N ), S ( U0N ) >,
          boost::mpl::apply1< ResultMetafunction, R >
      >
{ };

template< class R comma_class_T0N, class ResultMetafunction >
struct builtin_is_callable< R (*)( T0N ), void, ResultMetafunction >
    : boost::mpl::apply1< ResultMetafunction, R >
{ };

#undef comma_class_T0N
#undef comma_class_U0N
#undef T0N
#undef U0N

#undef N

#endif // #ifndef BOOST_PP_IS_ITERATING