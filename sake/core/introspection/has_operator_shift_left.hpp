/*******************************************************************************
 * core/introspection/has_operator_shift_left.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_shift_left<
 *     T, U,
 *     Result = void,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_SHIFT_LEFT_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_SHIFT_LEFT_HPP

#include <sake/core/introspection/has_operator_binary.hpp>
#include <sake/core/introspection/private/builtin_has_operator_shift.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_shift_left
#define SAKE_INTROSPECTION_OPERATOR_NAME <<
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultMetafunction ) \
    ::sake::introspection_private::builtin_has_operator_shift< T, U, Result, ResultMetafunction >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_SHIFT_LEFT_HPP
