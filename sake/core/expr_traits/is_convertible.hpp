/*******************************************************************************
 * core/expr_traits/is_convertible.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * #define SAKE_EXPR_IS_CONVERTIBLE( from_expression, to_type )
 *
 * This expands to an expression which evaluates (at compile-time) to true if
 * the type of from_expression is convertible to to_type.
 *
 * Note: Any type is convertible to (cv-qualified) void.
 * Note: from_expression must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP
#define SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/yes_no_type.hpp>

#define SAKE_EXPR_IS_CONVERTIBLE( from_expression, to_type ) \
    ( \
        sizeof( ::sake::yes_type ) \
     == sizeof( ::sake::expr_is_convertible_private::helper< to_type >::apply( from_expression ) ) \
    )

namespace sake
{

namespace expr_is_convertible_private
{

template<
    class T,
    bool = boost::is_object<T>::value ||
           boost_ext::is_reference<T>::value
>
struct helper;

template< class T >
struct helper< T, true >
{
    static sake::yes_type apply(T);
    static sake::no_type  apply(...);
};

template<>
struct helper< void, false >
{ static sake::yes_type apply(...); };

template<>
struct helper< void const, false >
{ static sake::yes_type apply(...); };

template<>
struct helper< void volatile, false >
{ static sake::yes_type apply(...); };

template<>
struct helper< void const volatile, false >
{ static sake::yes_type apply(...); };

template< class T >
struct helper< T, false >
{ static sake::no_type apply(...); };

#define test( from, to ) BOOST_STATIC_ASSERT( SAKE_EXPR_IS_CONVERTIBLE( (sake::declval< from >()), to ) );
test( int, int )
test( int, void )
test( int, long )
test( int*, void* )
#undef test
#define test( from, to ) BOOST_STATIC_ASSERT( !SAKE_EXPR_IS_CONVERTIBLE( (sake::declval< from >()), to ) );
test( int, void * )
test( void*, int )
test( void*, int* )
#undef test

} // namespace expr_is_convertible_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP
