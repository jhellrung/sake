/*******************************************************************************
 * core/introspection/has_member_function.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION()
 *
 * This defines an interface to allow one to easily define a has_mem_func
 * metafunction for a member function mem_func.  One defines certain
 * predetermined macros to specify the desired member function, then #include's
 * SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION().  This generates a
 * metafunction with signature
 *
 * template<
 *     class T,
 *     class Signature = [specified default],
 *     class ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 * struct has_mem_func.
 *
 * The macros to define to specify the desired member function are as follows:
 *
 * SAKE_INTROSPECTION_TRAIT_NAME
 *     Expands to the name of the trait to be defined (e.g., "has_mem_func").
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME
 *     Expands to the name of the member function (as it would appear in a
 *     declaration) (e.g., "mem_func").
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_SIGNATURE( T )
 *     [ defaults to "void" ]
 *     Expands to the default for the Signature template parameter.
 * SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS
 *     [ defaults to "( 0, SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY )" ]
 *     Expands to a Boost.Preprocessor 2-tuple giving the (inclusive) arity
 *     limits of the trait.  Special consideration is given to strictly unary
 *     member functions, e.g., operator[], to ensure this machinery works for
 *     them, too.
 * SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultMetafunction )
 *     [ defaults to "boost::false_type" ]
 *     Expands to a type with a nested type typedef indicating whether the
 *     builtin type T has the "member function" with the given signature
 *     (meaningful only for operators which must be overloaded as member
 *     functions, e.g., operator[]).  The result type of the "member function"
 *     should be convertible to the result type of Signature and satisfy the
 *     Boost.MPL metafunction class or lambda expression ResultMetafunction.
 *
 * Note that this doesn't work quite so well for nullary member functions.
 * (const) reference conversions of the result type can be picked up, but no
 * other converions can, so this case would have to be explicitly handled to
 * work like the polyary member function case.  See is_callable.
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_MEMBER_FUNCTION_HPP
#define SAKE_CORE_INTROSPECTION_HAS_MEMBER_FUNCTION_HPP

#include <boost/mpl/always.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/add_reference_add_const.hpp>
#include <sake/boost_ext/type_traits/is_builtin_object.hpp>
#include <sake/boost_ext/type_traits/propagate_qualifiers.hpp>
#include <sake/boost_ext/type_traits/remove_reference.hpp>

#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/expr_traits/is_convertible.hpp>
#include <sake/core/expr_traits/is_void.hpp>
#include <sake/core/introspection/private/dummy.hpp>
#include <sake/core/utility/convertible_from_any.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/extension.hpp>
#include <sake/core/utility/yes_no_type.hpp>

#ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY 8
#endif // #ifndef SAKE_INTROSPECTION_MEMBER_FUNCTION_DEFAULT_MAX_ARITY

#define SAKE_INTROSPECTION_DEFINE_HAS_MEMBER_FUNCTION() \
    <sake/core/introspection/private/has_member_function.ipp>

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_MEMBER_FUNCTION_HPP
