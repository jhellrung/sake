/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_assign.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_ASSIGN_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_ASSIGN_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class Signature, class ResultPred >
struct builtin_has_operator_assign_helper;

template< class T, class ResultPred >
struct builtin_has_operator_assign_helper< T, void, ResultPred >
    : boost::mpl::apply1< ResultPred, T& >
{ };

template< class T, class Result, class U, class ResultPred >
struct builtin_has_operator_assign_helper< T, Result ( U ), ResultPred >
    : boost_ext::mpl::and3<
          boost_ext::is_convertible< U, T const & >,
          boost_ext::is_convertible< T&, Result >,
          boost::mpl::apply1< ResultPred, T& >
      >
{ };

template< class T, class Signature, class ResultPred >
struct builtin_has_operator_assign
    : boost::false_type
{ };

template< class T, class Signature, class ResultPred >
struct builtin_has_operator_assign< T const, Signature, ResultPred >
    : boost::false_type
{ };

template< class T, class Signature, class ResultPred >
struct builtin_has_operator_assign< T&, Signature, ResultPred >
    : boost_ext::mpl::and2<
          boost::is_object<T>,
          builtin_has_operator_assign_helper< T, Signature, ResultPred >
      >
{ };

template< class T, class Signature, class ResultPred >
struct builtin_has_operator_assign< T const &, Signature, ResultPred >
    : boost::false_type
{ };

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_ASSIGN_HPP
