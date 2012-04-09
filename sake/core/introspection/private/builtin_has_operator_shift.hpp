/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_shift.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_SHIFT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_SHIFT_HPP

#include <boost/mpl/apply.hpp>
#include <boost/type_traits/integral_promotion.hpp>

#include <sake/boost_ext/mpl/and.hpp>
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
struct builtin_has_operator_shift_impl
    : boost_ext::mpl::and4<
          boost_ext::is_integral_or_enum<T>,
          boost_ext::is_integral_or_enum<U>,
          boost_ext::is_convertible<
              typename boost::integral_promotion<T>::type,
              Result
          >,
          boost::mpl::apply1<
              ResultPred,
              typename boost::integral_promotion<T>::type
          >
      >
{ };

template< class T, class U, class Result, class ResultPred >
struct builtin_has_operator_shift
    : builtin_has_operator_shift_impl<
          typename boost_ext::remove_qualifiers<T>::type,
          typename boost_ext::remove_qualifiers<U>::type,
          Result,
          ResultPred
      >
{ };

namespace
{

#ifdef _MSC_VER
#pragma warning ( push )
#pragma warning ( disable : 4804 ) // warning C4804: '<<'/'>>' : unsafe use of type 'bool' in operation
#endif // #ifdef _MSC_VER

#define test( T, op, U, Result ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply< Result >, \
        sake::declval<T>() op sake::declval<U>() \
    ) );
test( bool, <<, bool, int )
test( bool, <<, short, int )
test( bool, <<, int, int )
test( short, <<, bool, int )
test( short, <<, short, int )
test( short, <<, int, int )
test( int, <<, bool, int )
test( int, <<, short, int )
test( int, <<, int, int )
test( long, <<, int, long )
test( int, <<, long, int )
test( bool, >>, bool, int )
test( bool, >>, short, int )
test( bool, >>, int, int )
test( short, >>, bool, int )
test( short, >>, short, int )
test( short, >>, int, int )
test( int, >>, bool, int )
test( int, >>, short, int )
test( int, >>, int, int )
test( long, >>, int, long )
test( int, >>, long, int )
#undef test

#ifdef _MSC_VER
#pragma warning ( pop )
#endif // #ifdef _MSC_VER

} // namespace

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_SHIFT_HPP
