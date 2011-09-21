/*******************************************************************************
 * sake/boost_ext/type_traits/remove_reference_add_const.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::remove_reference_add_const<T>
 *
 * remove_reference_add_const<T> -> T const
 * remove_reference_add_const< T& > -> T
 * remove_reference_add_const< T&& > -> T
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_REFERENCE_ADD_CONST_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_REFERENCE_ADD_CONST_HPP

#include <boost/config.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct remove_reference_add_const { typedef T const type; };
template< class T > struct remove_reference_add_const< T& > { typedef T type; };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct remove_reference_add_const< T&& > { typedef T type; };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct remove_reference_add_const< boost::rv<T> & > { typedef T type; };
template< class T > struct remove_reference_add_const< boost::rv<T> const & > { typedef T const type; };
template< class T > struct remove_reference_add_const< boost::rv<T> volatile & > { typedef T volatile type; };
template< class T > struct remove_reference_add_const< boost::rv<T> const volatile & > { typedef T const volatile type; };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_REMOVE_REFERENCE_ADD_CONST_HPP
