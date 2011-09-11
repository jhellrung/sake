/*******************************************************************************
 * core/introspection/has_operator_shift_right.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_shift_right<
 *     T, U,
 *     Result = typename boost_ext::remove_qualifiers<T>::type,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_SHIFT_RIGHT_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_SHIFT_RIGHT_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/private/builtin_has_operator_shift.hpp>
#include <sake/core/introspection/has_operator_binary.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_shift_right
#define SAKE_INTROSPECTION_OPERATOR_NAME >>
#define SAKE_INTROSPECTION_DEFAULT_RESULT( T, U ) \
    typename ::sake::boost_ext::remove_qualifiers<T>::type
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultMetafunction ) \
    ::sake::introspection_private::builtin_has_operator_shift< T, U, Result, ResultMetafunction >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_SHIFT_RIGHT_HPP
