/*******************************************************************************
 * boost_ext/type_traits/add_rvalue_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_rvalue_reference<T>
 *
 * add_rvalue_reference<T> -> T&&
 * add_rvalue_reference< T& > -> T&
 * add_rvalue_reference< T const > -> T const
 *
 * Note that, contrary to the name, this will *not* add an rvalue reference to a
 * const-qualified type, as this is (almost) never useful.
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP

#include <boost/config.hpp>

#include <sake/core/move/is_movable.hpp>
#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

#ifndef BOOST_NO_RVALUE_REFERENCES

template< class T > struct add_rvalue_reference { typedef T&& type; };
template< class T > struct add_rvalue_reference< T const > { typedef T const type; };
template<> struct add_rvalue_reference< void > { typedef void type; };
template<> struct add_rvalue_reference< void const > { typedef void const type; };
template<> struct add_rvalue_reference< void volatile > { typedef void volatile type; };
template<> struct add_rvalue_reference< void const volatile > { typedef void const volatile type; };

#else // #ifndef BOOST_NO_RVALUE_REFERENCES

namespace add_rvalue_reference_private
{

template< class T, bool = sake::is_movable<T>::value >
struct impl;

template< class T >
struct impl< T, false >
{ typedef T type; };

template< class T >
struct impl< T, true >
{ typedef boost::rv<T>& type; };

} // namespace add_rvalue_reference_private

template< class T >
struct add_rvalue_reference
    : add_rvalue_reference_private::impl<T>
{ };

#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_RVALUE_REFERENCE_HPP
