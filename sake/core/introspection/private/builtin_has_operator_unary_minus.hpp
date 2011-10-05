/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_unary_minus.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_MINUS_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_MINUS_HPP

#include <boost/type_traits/integral_constant.hpp>

#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/private/builtin_has_operator_unary_sign.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Result, class ResultPred >
struct builtin_has_operator_unary_minus_impl
    : builtin_has_operator_unary_sign_impl< T, Result, ResultPred >
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_unary_minus_impl< T*, Result, ResultPred >
    : boost::false_type
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_unary_minus
    : builtin_has_operator_unary_minus_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          Result,
          ResultPred
      >
{ };

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_MINUS_HPP
