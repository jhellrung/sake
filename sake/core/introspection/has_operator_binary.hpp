/*******************************************************************************
 * core/introspection/has_operator_binary.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()
 *
 * This defines an interface to allow one to easily define has_operator_op
 * metafunctions for a binary operator op.  One defines certain predetermined
 * macros to specify the desired operator, then #include's
 * SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY().  This generates a
 * metafunction with signature
 *
 * template<
 *     class T, class U,
 *     class Result = [specified default],
 *     class ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 * struct has_operator_op.
 *
 * The macros to define to specify the desired operator are as follows:
 *
 * SAKE_INTROSPECTION_TRAIT_NAME
 *     Expands to the name of the trait to be defined (e.g., "has_operator_op"
 *     above).
 * SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, T, U )
 *     Expands to a declaration of the operator with result type Result and
 *     parameter types T and U (e.g., "Result operator op( T, U );").
 * SAKE_INTROSPECTION_OPERATOR_APPLY( x, y )
 *     Expands to an application of the operator with parameters x and y (e.g.,
 *     "x op y").
 * SAKE_INTROSPECTION_DEFAULT_RESULT( T, U )
 *     [ defaults to "void" ]
 *     Expands to the default for the Result template parameter of the trait.
 * SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultMetafunction )
 *     [ defaults to "boost::false_type" ]
 *     Expands to a type with a nested type typedef indicating whether the
 *     builtin types T and U have the operator (with the result convertible to
 *     Result and satisfying the Boost.MPL metafunction class or lambda
 *     expression ResultMetafunction).
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BINARY_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BINARY_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/is_builtin_object.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/private/dummy.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/convertible_from_any.hpp>
#include <sake/core/utility/extension.hpp>

#define SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY() \
    <sake/core/introspection/private/has_operator_binary.ipp>

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BINARY_HPP
