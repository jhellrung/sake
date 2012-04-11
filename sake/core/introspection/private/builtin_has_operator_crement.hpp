/*******************************************************************************
 * sake/core/introspection/private/builtin_has_operator_crement.hpp
 *
 * Copyright 2012, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 ******************************************************************************/

#ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_CREMENT_HPP
#define SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_CREMENT_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_object.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sake/boost_ext/mpl/curry_quote.hpp>
#include <sake/core/expr_traits/apply.hpp>
#include <sake/core/utility/declval.hpp>

namespace sake
{

namespace introspection_private
{

template< class T >
struct builtin_has_operator_crement_helper
    : boost::is_arithmetic<T>
{ };

template< class T >
struct builtin_has_operator_crement_helper< T const >
    : boost::false_type
{ };

template< class T >
struct builtin_has_operator_crement_helper< T* >
    : boost::is_object<T>
{ };

template< class T >
struct builtin_has_operator_crement
    : boost::false_type
{ };

template< class T >
struct builtin_has_operator_crement< T& >
    : builtin_has_operator_crement_helper<T>
{ };

namespace
{

#define test( op, T ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply< T& >::type, \
        op sake::declval< T& >() \
    ) );
test( ++, bool )
test( ++, int )
test( ++, int* )
test( --, int )
test( --, int* )
#undef test
#define test( T, op ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY( \
        boost_ext::mpl::curry_quote2< boost::is_same >::apply<T>::type, \
        sake::declval< T& >() op \
    ) );
// For some reason, the type of declval< bool& >()++ is bool& on MSVC9.
#if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
BOOST_STATIC_ASSERT( SAKE_EXPR_APPLY(
    typename boost_ext::mpl::curry_quote2< boost::is_same >::apply< bool& >::type,
    sake::declval< bool& >()++
) );
#else // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
test( bool, ++ )
#endif // #if SAKE_WORKAROUND_MSC_VERSION_LESS_EQUAL( 1500 )
test( int, ++ )
test( int*, ++ )
test( int, -- )
test( int*, -- )
#undef test

} // namespace

} // namespace introspection_private

} // namespace sake

#endif // #ifndef SAKE_CORE_INTROSPECTION_PRIVATE_BUILTIN_HAS_OPERATOR_CREMENT_HPP
