/*******************************************************************************
 * sake/core/introspection/has_operator_bracket.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_bracket<
 *     T, U,
 *     Result = void,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BRACKET_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BRACKET_HPP

#include <sake/core/introspection/is_callable_member_function.hpp>
#include <sake/core/introspection/private/builtin_has_operator_bracket.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME                   has_operator_bracket
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_NAME         operator[]
#define SAKE_INTROSPECTION_MEMBER_FUNCTION_ARITY_LIMITS ( 1, 1 )
#define SAKE_INTROSPECTION_BUILTIN_HAS_MEMBER_FUNCTION( T, Signature, ResultCond ) \
    ::sake::introspection_private::builtin_has_operator_bracket< T, Signature, ResultCond >
#include SAKE_INTROSPECTION_DEFINE_IS_CALLABLE_MEMBER_FUNCTION()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BRACKET_HPP
