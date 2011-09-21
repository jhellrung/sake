/*******************************************************************************
 * sake/core/introspection/has_operator_post_increment.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct has_operator_post_increment<
 *     T,
 *     Result = typename boost_ext::remove_qualifiers<T>::type,
 *     ResultMetafunction = boost::mpl::always< boost::true_type >
 * >
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_POST_INCREMENT_HPP
#define SAKE_CORE_INTROSPECTION_HAS_OPERATOR_POST_INCREMENT_HPP

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/private/builtin_has_operator_post_crement.hpp>
#include <sake/core/introspection/has_operator_unary.hpp>

namespace sake
{

#define SAKE_INTROSPECTION_TRAIT_NAME                    has_operator_post_increment
#define SAKE_INTROSPECTION_OPERATOR_DECLARE( Result, T ) Result operator++( T, int );
#define SAKE_INTROSPECTION_OPERATOR_APPLY( x )           x++
#define SAKE_INTROSPECTION_OPERATOR_DEFAULT_RESULT( T ) \
    typename ::sake::boost_ext::remove_qualifiers<T>::type
#define SAKE_INTROSPECTION_BUILTIN_HAS_OPERATOR( T, Result, ResultMetafunction ) \
    ::sake::introspection_private::builtin_has_operator_post_crement< T, Result, ResultMetafunction >
#include SAKE_INTROSPECTION_DEFINE_HAS_OPERATOR_UNARY()

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_HAS_OPERATOR_POST_INCREMENT_HPP
