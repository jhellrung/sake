/*******************************************************************************
 * sake/core/expr_traits/is_convertible.hpp
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

#include <boost/config.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/utility/enable_if.hpp>

#include <sake/boost_ext/mpl/or.hpp>
#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/move/rv.hpp>
#include <sake/core/utility/declval.hpp>
#include <sake/core/utility/true_false_tag.hpp>

#define SAKE_EXPR_IS_CONVERTIBLE( from_expression, to_type ) \
    (SAKE_SIZEOF_TRUE_TAG == sizeof( ::sake::expr_is_convertible_private::helper< to_type >::apply( from_expression ) ))

namespace sake
{

namespace expr_is_convertible_private
{

template<
    class T,
    bool = boost_ext::mpl::or2<
               boost_ext::is_reference<T>,
               boost::is_object<T>
           >::value
>
struct helper;

template< class T >
struct helper< T, true >
{
    static sake::true_tag  apply(T);
    static sake::false_tag apply(...);
};

#ifdef BOOST_NO_RVALUE_REFERENCES
// Explicitly disable any implicit T& -> boost::rv<T>& conversions.
template< class T >
struct helper< SAKE_RV_REF( T ), true >
{
    static sake::true_tag  apply(SAKE_RV_REF( T ));
    static sake::false_tag apply(...);
    template< class U >
    static typename boost::enable_if_c<
        boost::is_base_of<T,U>::value,
        sake::false_tag
    >::type apply(U&);
};
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

template<>
struct helper< void, false >
{ static sake::true_tag apply(...); };

template<>
struct helper< void const, false >
{ static sake::true_tag apply(...); };

template<>
struct helper< void volatile, false >
{ static sake::true_tag apply(...); };

template<>
struct helper< void const volatile, false >
{ static sake::true_tag apply(...); };

template< class T >
struct helper< T, false >
{ static sake::false_tag apply(...); };

namespace
{

#define test( from, to ) \
    BOOST_STATIC_ASSERT( SAKE_EXPR_IS_CONVERTIBLE( \
        (sake::declval< from >()), \
        to \
    ) );
test( int, int )
test( int, void )
test( int, long )
test( int*, void* )
#undef test
#define test( from, to ) \
    BOOST_STATIC_ASSERT( !SAKE_EXPR_IS_CONVERTIBLE( \
        (sake::declval< from >()), \
        to \
    ) );
test( int, void * )
test( void*, int )
test( void*, int* )
#undef test

} // namespace

} // namespace expr_is_convertible_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP
