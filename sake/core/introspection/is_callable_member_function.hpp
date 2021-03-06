/*******************************************************************************
 * sake/core/introspection/is_callable_member_function.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()
 *
 * This provides machinery to allow one to easily define a
 * is_callable_mem_fun_xxx metafunction, which determines (up to certain
 * language limitations) whether one can call a member function xxx on a given
 * object type with a given signature.  One defines certain predetermined macros
 * to specify the desired function, then #include's
 * SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION().  This generates a
 * metafunction with signature
 *
 * template<
 *     class T,
 *     class Signature = [specified default],
 *     class ResultPred = boost::mpl::always< boost::true_type >
 * >
 * struct is_callable_mem_fun_xxx.
 *
 * The macros to define to specify the desired member function are as follows:
 *
 * SAKE_INTROSPECTION_TRAIT_NAME
 *     Expands to the name of the trait to be defined (e.g.,
 *     "is_callable_mem_fun_xxx").
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
 *     Expands to the name of the member function (as it would appear in a
 *     declaration) (e.g., "xxx").
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T )
 *     [ defaults to "void" ]
 *     Expands to the default for the Signature template parameter.
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY
 *     [ optional ]
 *     Expands to the (fixed) arity of the member function.
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
 *     [ optional, defaults to "( 0, SAKE_INTROSPECTION_DEFAULT_MAX_ARITY )" ]
 *     Expands to a Boost.PP 2-tuple giving the (inclusive) arity limits of the
 *     member function.  Note that at most one of *_ARITY and *_ARITY_LIMITS
 *     should be defined.
 * SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultPred )
 *     [ defaults to "boost::false_type" ]
 *     Expands to a type with a nested type typedef indicating whether the
 *     builtin type T has the given "member function" callable with the given
 *     signature (meaningful only for operators which must be overloaded as
 *     member functions, e.g., operator[]).  The result type of calling the
 *     "member" should be convertible to the result type of Signature and
 *     satisfy the Boost.MPL metafunction class or lambda expression
 *     ResultPred.
 *
 * For member functions with a language-imposed fixed arity N (e.g., operator[]
 * or operator=), one should define *_ARITY as "N" to ensure that the member
 * function overloads defined by the implementation have a syntactically valid
 * signature.  Otherwise, it's better to define *_ARITY_LIMITS as "( N, N )".
 *
 * Note that this doesn't work quite so well for nullary member functions.
 * (const) reference conversions of the result type can be picked up, but no
 * other converions can, so this case would have to be explicitly handled (see
 * is_callable) to work like the polyary member function case.
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_MEMBER_FUNCTION_HPP
#define SAKE_CORE_INTROSPECTION_IS_CALLABLE_MEMBER_FUNCTION_HPP

#include <boost/config.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/preprocessor/keyword/operator.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_builtin_object.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_rvalue_reference.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/fwd.hpp>
#include <sake/core/introspection/private/check_result.hpp>
#include <sake/core/introspection/private/dummy.hpp>
#include <sake/core/utility/convertible_from_any.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/true_false_tag.hpp>

#define SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION() \
    <sake/core/introspection/private/is_callable_member_function.ipp>

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_MEMBER_FUNCTION_HPP
