/*******************************************************************************
 * sake/core/introspection/has_operator_star.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_star<
 *     T,
 *     Result = void,
 *     ResultPred = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_STAR_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_STAR_HPP

#include <sake/core/introspection/has_operator_unary.hpp>
#include <sake/core/introspection/private/builtin_has_operator_star.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME    has_operator_star
#define SAKE_INTROSPECTION_OPERATOR_NAME *
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, Result, ResultPred ) \
    ::sake::introspection_private::builtin_has_operator_star< T, Result, ResultPred >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_UNARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_STAR_HPP
