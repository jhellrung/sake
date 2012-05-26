/*******************************************************************************
 * sake/core/introspection/has_operator_equal.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_equal<
 *     T, U = T,
 *     Result = bool,
 *     ResultPred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_EQUAL_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_EQUAL_HPP

#include <sake/core/introspection/has_operator_binary.hpp>
#include <sake/core/introspection/private/builtin_has_operator_compare.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_equal
#define SAKE_INTROSPECTION_OPERATOR_NAME ==
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ) bool
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultPred ) \
    ::sake::introspection_private::builtin_has_operator_compare< T, U, Result, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_EQUAL_HPP
