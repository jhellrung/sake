/*******************************************************************************
 * sake/core/introspection/is_callable_function.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION()
 *
 * This provides machinery to allow one to easily define a is_callable_xxx
 * metafunction, which determines (up to certain language limitations) whether
 * one can call a (free, unqualified) function xxx with a given signature.  One
 * defines certain predetermined macros to specify the desired function, then
 * #include's SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION().  This generates
 * a metafunction with signature
 *
 * template<
 *     class Signature,
 *     class ResultPred = boost::mpl::always< boost::true_type >
 * >
 * struct is_callable_xxx.
 *
 * The macros to define to specify the desired function are as follows:
 *
 * SAKE_INTROSPECTION_TRAIT_NAME
 *     Expands to the name of the trait to be defined (e.g., "is_callable_xxx").
 * SAKE_INTROSPECTION_FUNCTION_NAME
 *     [ optional ]
 *     If defined, expands to the name of the function (e.g., "xxx").
 * SAKE_INTROSPECTION_FUNCTION_DECLARE( Result, T_tuple )
 *     [ defaults to "Result SAKE_INTROSPECTION_FUNCTION_NAME T_tuple ;" ]
 *     Expands to a declaration of the function with result type Result and
 *     parameter types given by the Boost.PP tuple T_tuple.
 * SAKE_INTROSPECTION_FUNCTION_APPLY( x_args )
 *     [ defaults to "SAKE_INTROSPECTION_FUNCTION_NAME x_args" ]
 *     Expands to an application of the function with parameters given by the
 *     parenthesized argument list x_args.
 * SAKE_INTROSPECTION_FUNCTION_ARITY
 *     [ optional ]
 *     Expands to the (fixed) arity of the function.
 * SAKE_INTROSPECTION_FUNCTION_ARITY_LIMITS
 *     [ optional, defaults to "( 1, SAKE_INTROSPECTION_DEFAULT_MAX_ARITY )" ]
 *     Expands to a Boost.PP 2-tuple giving the (inclusive) arity limits of the
 *     function.  Note that at most one of *_ARITY and *_ARITY_LIMITS should be
 *     defined.
 *
 * For functions with a language-imposed fixed arity N (e.g., operators), one
 * one should define *_ARITY as "N" to ensure that the member function overloads
 * defined by the implementation have a syntactically valid signature.
 * Otherwise, it's better to define *_ARITY_LIMITS as "( N, N )".
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_FUNCTION_HPP
#define SAKE_CORE_INTROSPECTION_IS_CALLABLE_FUNCTION_HPP

#include <boost/config.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/mpl/and.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/fwd.hpp>
#include <sake/core/introspection/private/dummy.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/convertible_from_any.hpp>

#define SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_FUNCTION() \
    <sake/core/introspection/private/is_callable_function.ipp>

#endif // #ifndef SAKE_CORE_INTROSPECTION_IS_CALLABLE_FUNCTION_HPP
