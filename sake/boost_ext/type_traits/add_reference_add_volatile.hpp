/*******************************************************************************
 * sake/boost_ext/type_traits/add_reference_add_volatile.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_reference_add_volatile<T>
 *
 * add_reference_add_volatile<T> -> T volatile &
 * add_reference_add_volatile< T& > -> T&
 * add_reference_add_volatile< T&& > -> T&&
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_REFERENCE_ADD_VOLATILE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_REFERENCE_ADD_VOLATILE_HPP

#include <boost/config.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct add_reference_add_volatile { typedef T volatile & type; };
template< class T > struct add_reference_add_volatile< T& > { typedef T& type; };
template<> struct add_reference_add_volatile< void > { typedef void volatile type; };
template<> struct add_reference_add_volatile< void const > { typedef void const volatile type; };
template<> struct add_reference_add_volatile< void volatile > { typedef void volatile type; };
template<> struct add_reference_add_volatile< void const volatile > { typedef void const volatile type; };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct add_reference_add_volatile< T&& > { typedef T&& type; };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct add_reference_add_volatile< boost::rv<T> & > { typedef boost::rv<T> & type; };
template< class T > struct add_reference_add_volatile< boost::rv<T> const & > { typedef boost::rv<T> const & type; };
template< class T > struct add_reference_add_volatile< boost::rv<T> volatile & > { typedef boost::rv<T> volatile & type; };
template< class T > struct add_reference_add_volatile< boost::rv<T> const volatile & > { typedef boost::rv<T> const volatile & type; };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_REFERENCE_ADD_VOLATILE_HPP
