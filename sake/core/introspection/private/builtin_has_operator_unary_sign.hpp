/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_unary_sign.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_SIGN_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_SIGN_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_promotion.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include <sake/boost_ext/mpl/and.hpp>
#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_arithmetic_or_enum.hpp>
#include <sake/boost_ext/type_traits/is_convertible.hpp>
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

template< class T, class Result, class ResultPred >
struct builtin_has_operator_unary_sign_impl
    : boost_ext::mpl::and3<
          boost_ext::mpl::or2<
              boost_ext::is_arithmetic_or_enum<T>,
              boost::is_pointer<T>
          >,
          boost_ext::is_convertible< typename boost::integral_promotion<T>::type, Result >,
          boost::mpl::apply1< ResultPred, typename boost::integral_promotion<T>::type >
      >
{ };

template< class T, class Result, class ResultPred >
struct builtin_has_operator_unary_sign
    : builtin_has_operator_unary_sign_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          Result,
          ResultPred
      >
{ };

namespace
{

#define test( op, T, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply< Result >::type, \
        op sake::declval<T>() \
    ) );
test( +, bool, int )
test( +, short, int )
test( +, int, int )
test( +, unsigned int, unsigned int )
test( +, void*, void* )
#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable : 4804 ) // '-' : unsafe use of type 'bool' in operation
#pragma warning ( disable : 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#endif // #ifdef _MSC_VER
test( -, bool, int )
test( -, short, int )
test( -, int, int )
test( -, unsigned int, unsigned int )
#ifdef _MSC_VER
#pragma warning ( pop )
#endif // #ifdef _MSC_VER
#undef test

} // namespace

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_UNARY_SIGN_HPP
