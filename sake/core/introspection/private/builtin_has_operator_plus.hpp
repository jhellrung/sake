/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_plus.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PLUS_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PLUS_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

#include <sake/core/introspection/private/builtin_has_operator_arithmetic.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace introspection_private
{

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_plus_dispatch
    : builtin_has_operator_arithmetic_impl< T, U, Result, ResultPred >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_plus_dispatch< T*, U, Result, ResultPred >
    : boost_ext::mpl::and4<
          boost::is_object<T>,
          boost_ext::is_integral_or_enum<U>,
          boost_ext::is_convertible< T*, Result >,
          boost::mpl::apply1< ResultPred, T* >
      >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_plus_dispatch< T, U*, Result, ResultPred >
    : builtin_has_operator_plus_dispatch< U*, T, Result, ResultPred >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_plus_dispatch< T*, U*, Result, ResultPred >
    : boost::false_type
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_plus
    : builtin_has_operator_plus_dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultPred
      >
{ };

namespace
{

#define test( T, U ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply< T* >, \
        sake::declval< T* >() + sake::declval<U>() \
    ) );
test( int*, bool )
test( int*, int )
test( int*, unsigned int )
test( int const *, bool )
test( int const *, int )
test( int const *, unsigned int )
#undef test

} // namespace

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_PLUS_HPP
