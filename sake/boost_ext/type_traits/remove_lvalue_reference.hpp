/*******************************************************************************
 * sake/boost_ext/type_traits/remove_lvalue_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::remove_lvalue_reference<T>
 *
 * remove_lvalue_reference<T> -> T
 * remove_lvalue_reference< T& > -> T
 * remove_lvalue_reference< T&& > -> T&&
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_LVALUE_REFERENCE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_LVALUE_REFERENCE_HPP

#include <boost/config.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct remove_lvalue_reference { typedef T type; };
template< class T > struct remove_lvalue_reference< T& > { typedef T type; };
#ifdef BOOST_NO_RVALUE_REFERENCES
template< class T > struct remove_lvalue_reference< boost::rv<T> & > { typedef boost::rv<T> & type; };
template< class T > struct remove_lvalue_reference< boost::rv<T> const & > { typedef boost::rv<T> const & type; };
template< class T > struct remove_lvalue_reference< boost::rv<T> volatile & > { typedef boost::rv<T> volatile & type; };
template< class T > struct remove_lvalue_reference< boost::rv<T> const volatile & > { typedef boost::rv<T> const volatile & type; };
#endif // #ifdef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_LVALUE_REFERENCE_HPP
