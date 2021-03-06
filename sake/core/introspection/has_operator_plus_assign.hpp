/*******************************************************************************
 * sake/core/introspection/has_operator_plus_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_plus_assign<
 *     T, U = T,
 *     Result = typename boost_ext::add_reference<T>::type,
 *     ResultPred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_PLUS_ASSIGN_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_PLUS_ASSIGN_HPP

#include <sake/boost_ext/type_traits/add_reference.hpp>

#include <sake/core/introspection/has_operator_binary.hpp>
#include <sake/core/introspection/private/builtin_has_operator_plus.hpp>
#include <sake/core/introspection/private/builtin_has_operator_op_assign.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_plus_assign
#define SAKE_INTROSPECTION_OPERATOR_NAME +=
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ) \
    typename ::sake::boost_ext::add_reference<T>::type
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultPred ) \
    ::sake::introspection_private::builtin_has_operator_op_assign< \
        ::sake::introspection_private::builtin_has_operator_plus \
    >::apply< T, U, Result, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_PLUS_ASSIGN_HPP
