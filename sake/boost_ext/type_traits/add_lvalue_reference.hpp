/*******************************************************************************
 * boost_ext/type_traits/add_lvalue_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_lvalue_reference<T>
 *
 * add_lvalue_reference<T> -> T&
 * add_lvalue_reference< T& > -> T&
 * add_lvalue_reference< T&& > -> T&
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP

#include <boost/config.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct add_lvalue_reference { typedef T& type; };
template<> struct add_lvalue_reference< void > { typedef void type; };
template<> struct add_lvalue_reference< void const > { typedef void const type; };
template<> struct add_lvalue_reference< void volatile > { typedef void volatile type; };
template<> struct add_lvalue_reference< void const volatile > { typedef void const volatile type; };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct add_lvalue_reference< T&& > { typedef T& type; };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct add_lvalue_reference< T& > { typedef T& type; };
template< class T > struct add_lvalue_reference< boost::rv<T> > { typedef T & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> const > { typedef T const & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> volatile > { typedef T volatile & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> const volatile > { typedef T const volatile & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> & > { typedef T & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> const & > { typedef T const & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> volatile & > { typedef T volatile & type; };
template< class T > struct add_lvalue_reference< boost::rv<T> const volatile & > { typedef T const volatile & type; };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_LVALUE_REFERENCE_HPP
