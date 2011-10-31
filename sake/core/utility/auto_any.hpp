/*******************************************************************************
 * sake/core/utility/auto_any.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct auto_any_base
 * typedef ... auto_any_t
 * struct auto_any<T>
 * auto_any_cast<T>(auto_any_t x) -> boost_ext::add_reference<T>::type
 * auto_capture(type_tag<T>, boost::remove_const<T>::type x) -> auto_any<T>
 *
 * These allow the result of an expression to be captured without actually
 * knowing the *type* of the expression.  This can be very useful for macros in
 * the absence of any kind of "typeof" utility.
 ******************************************************************************/

#ifndef SAKE_CORE_UTILITY_AUTO_ANY_HPP
#define SAKE_CORE_UTILITY_AUTO_ANY_HPP

#include <boost/type_traits/remove_const.hpp>

#include <sake/boost_ext/type_traits/add_reference.hpp>

#include <sake/core/move/move.hpp>
#include <sake/core/move/rv.hpp>
#include <sake/core/utility/type_tag.hpp>

namespace sake
{

struct auto_any_base
// The bool conversion operator allows an auto_any_base variable to be declared
// within an if conditional (useful for macros to limit the scope of said
// variable declaration).
{ operator bool() const { return false; } };

typedef auto_any_base const & auto_any_t;

template< class T >
struct auto_any
    : auto_any_base
{
    mutable T value;
    auto_any(T const & x) : value(x) { }
    auto_any(SAKE_RV_REF( T ) x) : value(sake::move(x)) { }
};

template< class T >
struct auto_any< T const >
    : auto_any_base
{
    T const value;
    auto_any(T const & x) : value(x) { }
    auto_any(SAKE_RV_REF( T ) x) : value(sake::move(x)) { }
};

template< class T >
struct auto_any< T& >
    : auto_any_base
{
    T& value;
    auto_any(T& x) : value(x) { }
};

template< class T >
inline typename boost_ext::add_reference<T>::type
auto_any_cast(sake::auto_any_t x)
{ return static_cast< auto_any<T> const & >(x).value; }

template< class T >
inline sake::auto_any<T>
auto_capture(sake::type_tag<T>, typename boost::remove_const<T>::type x)
{ return sake::move(x); }

} // namespace sake

#endif // #ifndef SAKE_CORE_UTILITY_AUTO_ANY_HPP
