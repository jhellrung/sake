/*******************************************************************************
 * core/introspection/has_operator_logical_or.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_logical_or<
 *     T, U,
 *     Result = bool,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_LOGICAL_OR_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_LOGICAL_OR_HPP

#include <sake/core/introspection/has_operator_binary.hpp>
#include <sake/core/introspection/private/builtin_has_operator_binary_logical.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_logical_or
#define SAKE_INTROSPECTION_OPERATOR_NAME ||
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ) bool
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultMetafunction ) \
    ::sake::introspection_private::builtin_has_operator_binary_logical< T, U, Result, ResultMetafunction >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_LOGICAL_OR_HPP
