/*******************************************************************************
 * sake/core/introspection/has_operator_bit_or.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_bit_or<
 *     T, U = T,
 *     Result = typename boost_ext::common_result_type<
 *         typename boost_ext::remove_qualifiers<T>::type,
 *         typename boost_ext::remove_qualifiers<U>::type
 *     >::type,
 *     ResultPred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BIT_OR_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BIT_OR_HPP

#include <sake/boost_ext/type_traits/common_result_type.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/has_operator_binary.hpp>
#include <sake/core/introspection/private/builtin_has_operator_binary_bit.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_bit_or
#define SAKE_INTROSPECTION_OPERATOR_NAME |
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T, U ) \
    typename boost_ext::common_result_type< \
        typename boost_ext::remove_qualifiers<T>::type, \
        typename boost_ext::remove_qualifiers<U>::type \
    >::type
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, U, Result, ResultPred ) \
    ::sake::introspection_private::builtin_has_operator_binary_bit< T, U, Result, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_BINARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_BIT_OR_HPP
