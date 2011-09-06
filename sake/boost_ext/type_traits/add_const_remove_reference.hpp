/*******************************************************************************
 * boost_ext/type_traits/add_const_remove_reference.hpp
 *
 * Copyright 2011, Jeffrey Hellrung.
 * Distributed under the Boost Software License, Version 1.0.  (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * struct boost_ext::add_const_remove_reference<T>
 *
 * add_const_remove_reference<T> -> T const
 * add_const_remove_reference< T& > -> T const
 * add_const_remove_reference< T&& > -> T const
 ******************************************************************************/

#ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_REMOVE_REFERENCE_HPP
#define SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_REMOVE_REFERENCE_HPP

#include <boost/config.hpp>

#include <sake/core/move/rv.hpp>

namespace sake
{

namespace boost_ext
{

template< class T > struct add_const_remove_reference { typedef T const type; };
template< class T > struct add_const_remove_reference< T& > { typedef T const type; };
#ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct add_const_remove_reference< T&& > { typedef T const type; };
#else // #ifndef BOOST_NO_RVALUE_REFERENCES
template< class T > struct add_const_remove_reference< boost::rv<T> & > { typedef T const type; };
template< class T > struct add_const_remove_reference< boost::rv<T> const & > { typedef T const type; };
template< class T > struct add_const_remove_reference< boost::rv<T> volatile & > { typedef T const volatile type; };
template< class T > struct add_const_remove_reference< boost::rv<T> volatile const & > { typedef T const volatile type; };
#endif // #ifndef BOOST_NO_RVALUE_REFERENCES

} // namespace boost_ext

} // namespace sake

#endif // #ifndef SAKE_BOOST_EXT_TYPE_TRAITS_ADD_CONST_REMOVE_REFERENCE_HPP
