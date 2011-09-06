/*******************************************************************************
 * core/expr_traits/is_convertible.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * SAKE_EXPR_IS_CONVERTIBLE( FromExpr, ToType )
 *
 * This expands to an expression which evaluates (at compile-time) to true if
 * the type of FromExpr is convertible to ToType.
 *
 * Note: Any type is convertible to (cv-qualified) void.
 * Note: FromExpr must have non-void type.
 ******************************************************************************/

#ifndef SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP
#define SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP

#include <boost/type_traits/is_object.hpp>

#include <sake/boost_ext/type_traits/is_reference.hpp>

#include <sake/core/utility/yes_no_type.hpp>

#define SAKE_EXPR_IS_CONVERTIBLE( FromExpr, ToType ) \
    ( sizeof( ::sake::yes_type ) == sizeof( ::sake::expr_is_convertible_private::helper< ToType >::apply( FromExpr ) ) )

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

} // namespace expr_is_convertible_private

} // namespace sake

#endif // #ifndef SAKE_CORE_EXPR_TRAITS_IS_CONVERTIBLE_HPP
