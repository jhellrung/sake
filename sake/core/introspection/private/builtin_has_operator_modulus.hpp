/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_modulus.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_MODULUS_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_MODULUS_HPP

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/private/builtin_has_operator_arithmetic.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_modulus_impl
    : boost_ext::mpl::and3<
          boost_ext::is_integral_or_enum<T>,
          boost_ext::is_integral_or_enum<U>,
          builtin_has_operator_arithmetic_dispatch< T, U, Result, ResultPred >
      >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_modulus
    : builtin_has_operator_modulus_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultPred
      >
{ };

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_MODULUS_HPP
