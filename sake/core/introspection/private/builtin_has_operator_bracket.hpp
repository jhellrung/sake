/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_bracket.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_BRACKET_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_BRACKET_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/type_traits/add_reference.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
#include <sake/boost_ext/type_traits/is_integral_or_enum.hpp>
#include <sake/boost_ext/type_traits/remove_qualifiers.hpp>

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
struct builtin_has_operator_bracket_dispatch
    : boost::false_type
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_bracket_dispatch< T*, U, Result, ResultPred >
    : boost_ext::mpl::and4<
          boost::is_object<T>,
          boost_ext::is_integral_or_enum<U>,
          boost_ext::is_convertible< typename boost_ext::add_reference<T>::type, Result >,
          boost::mpl::apply1< ResultPred, typename boost_ext::add_reference<T>::type >
      >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_bracket_dispatch< T, U*, Result, ResultPred >
    : builtin_has_operator_bracket_dispatch< U*, T, Result, ResultPred >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_bracket_dispatch< T*, U*, Result, ResultPred >
    : boost::false_type
{ };

template< class T, class Signature, class ResultPred >
struct builtin_has_operator_bracket
    : boost::false_type
{ };

template< class T, class ResultPred >
struct builtin_has_operator_bracket< T const, void, ResultPred >
    : builtin_has_operator_bracket< T, void, ResultPred >
{ };
template< class T, class ResultPred >
struct builtin_has_operator_bracket< T&, void, ResultPred >
    : builtin_has_operator_bracket< T, void, ResultPred >
{ };
template< class T, class ResultPred >
struct builtin_has_operator_bracket< T*, void, ResultPred >
    : boost_ext::mpl::and2<
          boost::is_object<T>,
          boost::mpl::apply1< ResultPred, typename boost_ext::add_reference<T>::type >
      >
{ };

template< class T, class Result, class U, class ResultPred >
struct builtin_has_operator_bracket< T, Result ( U ), ResultPred >
    : builtin_has_operator_bracket_dispatch<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultPred
      >
{ };

namespace
{

#define test( T, U, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply< Result >::type, \
        sake::declval<T>() [ sake::declval<U>() ] \
    ) );
test( int*, int, int& )
test( int const *, int, int const & )
test( int, int*, int& )
test( int, int const *, int const & )
#undef test

} // namespace

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_BRACKET_HPP
