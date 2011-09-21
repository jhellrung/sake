/*******************************************************************************
 * sake/core/introspection/has_operator_unary.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_UNARY()
 *
 * This defines an interface to allow one to easily define a has_operator_op
 * metafunction for a unary operator op.  One defines certain predetermined
 * macros to specify the desired operator, then #include's
 * SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_UNARY().  This generates a
 * metafunction with signature
 *
 * template<
 *     class T,
 *     class Result = [specified default],
 *     class ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 * struct has_operator_op.
 *
 * The macros to define to specify the desired operator are as follows:
 *
 * SAKE_INTROSPECTION_TRAIT_NAME
 *     Expands to the name of the trait to be defined (e.g., "has_operator_op").
 * SAKE_INTROSPECTION_OPERATOR_NAME
 *     [ optional ]
 *     Expands to the name of the operator (e.g., "op").
 * SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, T )
 *     [ defaults to "Result operator SAKE_INTROSPECTION_OPERATOR_NAME (T);" ]
 *     Expands to a declaration of the operator with result type Result and
 *     parameter type T (e.g., "Result operator op( T );").
 * SAKE_INTROSPECTION_OPERATOR_APPLY( x )
 *     [ defaults to "SAKE_INTROSPECTION_OPERATOR_NAME x" ]
 *     Expands to an application of the operator with parameter x (e.g.,
 *     "op x" or "x op" for prefix or postfix operators, respectively).
 * SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T )
 *     [ defaults to "void" ]
 *     Expands to the default for the Result template parameter of the trait.
 * SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, Result, ResultMetafunction )
 *     [ defaults to "boost::false_type" ]
 *     Expands to a type with a nested type typedef indicating whether the
 *     builtin type T has the operator (with the result convertible to Result
 *     and satisfying the Boost.MPL metafunction class or lambda expression
 *     ResultMetafunction).
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_UNARY_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_UNARY_HPP

#include <boost/mpl/always.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_builtin_object.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/introspection/is_callable_function.hpp>
#include <sake/core/utility/extension.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_UNARY() \
    <sake/core/introspection/private/has_operator_unary.ipp>

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_UNARY_HPP
